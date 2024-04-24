#include "Serveur.hpp"

//Creer la socket utilisant le protocol AF_INET(internet ipv4) de type stream (TCP/IP)
//on la met en mode SO_REUSEADDR qui lui permet de reutiliser une adreese local
//option level a 1 for ip
size_t nb_client = 0;

void	Serveur::create_socket()
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
		throw std::runtime_error("Error: Cannot creat socket");
	int opt = 1;
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("Error: Cannot set socket");
}

//une fois la socket creer on va la lier a une adresse local
// et a son port de refence, 
// le flags INADDR_ANY specifie qu elle va recevoir des de n importe qu elle addresse ip
// htons rearange l ordre des bytes pour convenir au network byte order

void	Serveur::bind_socket()
{
	std::memset(&_sock_addr, 0, sizeof(struct sockaddr_in));
	std::cout << "port = " << _port << std::endl;
	_sock_addr.sin_family = AF_INET;
	_sock_addr.sin_addr.s_addr = INADDR_ANY;
	_sock_addr.sin_port = htons(_port);
	if (bind(_socket_fd, (struct sockaddr *)&_sock_addr, sizeof(_sock_addr)) == -1)
		throw std::runtime_error("Error: Cannot bind socket");
}

void	Serveur::create_epoll()
{
	_epoll_fd = epoll_create(1);
	if (_epoll_fd == -1)
		throw std::runtime_error("Error: Cannot create epoll");
	_event.events = EPOLLIN;
	_event.data.fd = _socket_fd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _socket_fd, &_event) == -1)
		throw std::runtime_error("Error: epoll_ctl");
}

Serveur::Serveur(const int &port, const std::string &password) : _port(port), _password(password)
{
	create_socket();
	bind_socket();
	if (listen(_socket_fd, SIZE_QUEUE) == 1)
		throw std::runtime_error("Error: Cannot listen to socket");
	create_epoll();
	std::time(&_date_lancement);
	std::cout << RPL_WELCOM("kiki") << std::endl; 
}

void	Serveur::create_client()
{
	int				client_fd;
	struct sockaddr_in cli_sock_addr;
	socklen_t		addrlen;

	std::memset(&cli_sock_addr, 0, sizeof(cli_sock_addr));
	addrlen = sizeof(cli_sock_addr);
	client_fd = accept(_socket_fd, (struct sockaddr *)&cli_sock_addr, &addrlen);
	if (client_fd == -1)
		return (run_error("Cannot accept new connection: "));
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(client_fd);
		return (run_error("Cannot accept new connection: "));
	}
	if (getsockname(client_fd, (struct sockaddr *)&cli_sock_addr, &addrlen))
		return (run_error("Error: getsockname"));
	_event.events = EPOLLIN;// | EPOLLOUT;
	_event.data.fd = client_fd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &_event) == -1)
	{
		close(client_fd);
		return (run_error("Error: epoll_ctl"));
	}
	Client *client = new(std::nothrow) Client(client_fd, cli_sock_addr);
	if (client == NULL)
	{
		epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
		close(client_fd);
		return (run_error("Error: new Client"));
	}
	nb_client++;
	_list_clients.insert(std::pair<int, Client *>(client_fd, client));
}

void	Serveur::remove_client(int fd)
{
	std::cout << "in remove client" << std::endl;
	delete _list_clients[fd];
	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	_list_clients.erase(fd); 
}

message_t	Serveur::parse_buff(const std::string &buffer, size_t &debut, size_t posn, size_t posr)
{
	//identifier les element de la commande (cmds et les parametres)
	// et les decouper 
	//il peut y avoir plusieur commande et une commande incomplete
	message_t	msg;
	size_t		limite;
	size_t		pos;

	while (buffer[debut] == '\r' || buffer[debut] == '\n')
		debut++;
	pos = buffer.find(' ', debut);
	limite = (posn < posr ? posn : posr);
	if (limite > buffer.size())
		return msg;
	if (limite < pos)
		pos = limite;
	msg.commande = buffer.substr(debut, pos - debut);
	debut = pos + 1;
	while (debut < limite)
	{
		if (buffer[debut] == ':')
		{
			msg.parametres.push_back(buffer.substr(debut + 1, limite - debut - 1));
			debut = limite;
			break ;
		}
		pos = buffer.find(' ', debut);
		if (limite < pos)
			pos = limite;
		msg.parametres.push_back(buffer.substr(debut, pos - debut));
		debut = pos + 1;
	}
	while (buffer[debut] == '\r' || buffer[debut] == '\n')
		debut++;
	return (msg);
}

void	print_message(message_t &msg)
{
	if (msg.commande == "")
		return ;
	std::cout << "-------message---------------------------------" << std::endl;
	std::cout << msg.commande << std::endl;
	for (size_t i = 0; i < msg.parametres.size(); i++)
		std::cout << "x " << msg.parametres[i] << std::endl;
}

void	Serveur::handle_cmds(int i)
{
	char	buffer[512];
	int		count;
	Client	*client =  _list_clients[_events_list[i].data.fd];

	count = recv(client->getSock_fd(), buffer, 512, MSG_DONTWAIT);
	if (count == -1)
		return (run_error("Cannot read in socket: "));
	if (count == 0)
		return (remove_client(client->getSock_fd()));
	buffer[count] = 0;
	if (count == 512 && (buffer[count] != '\r' || buffer[count] != '\n'))
	{
		if (client->sendMsg(ERR_INPUTOOLONG(client->getNickname())) == -1)
		{
			return (remove_client(_events_list[i].data.fd));
		}
	}
	else
	{
		//std::cout << "Buffer : " << buffer;
		if (!client->setInput_buf(client->getInput_buf() + buffer))
		{
			std::cout << client->getInput_buf();
			const std::string input = client->getInput_buf();
			size_t		posn = input.find('\n');
			size_t		posr = input.find('\r');
			size_t		debut = 0;
			message_t	msg;
			while (posn != std::string::npos || posr != std::string::npos)
			{
		 		msg = parse_buff(input, debut, posn, posr);
				// if (exec_commande(msg))
				// 	;
				posn = input.find('\n', debut);
				posr = input.find('\r', debut);
				print_message(msg);

			}
			client->clearBuf(); // peut etre garde le reste si derniere commande incomplete
		}
	}
}

void	Serveur::run_serveur()
{
	int	num_events;
	int	i;
	in_addr_t ad;
	struct hostent *host;

	ad = inet_addr("127.0.0.1");
	host = gethostbyaddr((const char *)&ad, sizeof(struct in_addr), AF_INET);
	if (host == NULL)
		throw std::runtime_error("gethostbyaddr");
	std::cout << "hostent : \n" << "h_name = " << host->h_name << std::endl;
	std::cout << "h_aliases = " << host->h_aliases << std::endl;
	std::cout << "h_addrtype = " << host->h_addrtype << std::endl; 

	while (g_run)
	{
		num_events = epoll_wait(_epoll_fd, _events_list, MAX_EVENTS, -1);
		if (num_events == -1)
		{
			if (g_run)
				throw std::runtime_error("Error: epoll_wait");
			break ;
		}
		for (i = 0; i < num_events; ++i)
		{
			if (_events_list[i].data.fd == _socket_fd)
				create_client();
			else
				handle_cmds(i);
		}
	}
}

Serveur::~Serveur()
{
	std::cout << nb_client << std::endl;
	if (_socket_fd > 0)
	{
		if (close(_socket_fd) == -1)
			throw std::runtime_error("Error: Cannot close socket fd");
	}
	for (size_t i = 0; i < _list_clients.size(); i++)
		delete _list_clients[i];
	if (_epoll_fd > 0)
	{
		if (close(_epoll_fd) == -1)
			throw std::runtime_error("Error: Cannot close socket fd");
	}
}

void	run_error(std::string msg)
{
	std::cerr << "Error: ";
	perror(msg.c_str());
}

const std::string&	Serveur::getPass() const
{
	return _password;
}
