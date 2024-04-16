#include "Serveur.hpp"


//Creer la socket utilisant le protocol AF_INET(internet ipv4) de type stream (TCP/IP)
//on la met en mode SO_REUSEADDR qui lui permet de reutiliser une adreese local
//option level a 1 for ip

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
	if (fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK) == -1)
	{
		close(client_fd);
		return (run_error("Cannot accept new connection: "));
	}

	//if (!getsockname(client_fd, (struct sockaddr *)&cli_sock_addr, &addrlen))

	_event.events = EPOLLIN;
	_event.data.fd = client_fd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &_event) == -1)
	{
		close(client_fd);
		return (run_error("Error: epoll_ctl"));
	}
	Client *client = new(std::nothrow) Client(client_fd);
	if (client == NULL)
	{
		epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
		close(client_fd);
		return (run_error("Error: new Client"));
	}
	_list_clients.insert(std::pair<int, Client *>(client_fd, client));
}

void	Serveur::remove_client(int fd)
{
	delete _list_clients[fd];
	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	_list_clients.erase(fd);
}

void	Serveur::handle_cmds(int i)
{
	char	buffer[512];
	int		count = 1;

	std::cout << "------------------------------------------------------\n";
	count = recv(_events_list[i].data.fd, buffer, 512, MSG_DONTWAIT);
	if (count == -1)
		return (run_error("Cannot read in socket: "));
	if (count == 0)
		return (remove_client(_events_list[i].data.fd));
	buffer[count] = 0;
	std::cout << buffer << std::endl;
	std::cout << (int)buffer[count - 1] << std::endl;

	//la fonction evra renvoye un msg au client lui disant que sa commande est trop longue
	if (count == 512 && (buffer[count] != '\r' || buffer[count] != '\n'))
		send(_events_list[i].data.fd, "erreur\n", 7, MSG_DONTWAIT);

	
}

void	Serveur::run_serveur()
{
	int	num_events;
	int	i;

	struct in_addr addr;
	struct hostent *host;

	inet_aton("127.0.0.1", &addr);
	host = gethostbyaddr((const char *)&addr, sizeof(struct in_addr), AF_INET);
	if (host == NULL)
		throw std::runtime_error("gethostbyaddr");
	std::cout << "hostent : \n" << "h_name = " << host->h_name << std::endl;
	std::cout << "h_aliases = " << host->h_aliases << std::endl;
	std::cout << "h_addrtype = " << host->h_addrtype << std::endl; 

	while (g_run)
	{
		num_events = epoll_wait(_epoll_fd, _events_list, MAX_EVENTS, -1);
		if (num_events == -1)
			throw std::runtime_error("Error: epoll_wait");
		for (i = 0; i < num_events; ++i)
		{
			if (_events_list[i].data.fd == _socket_fd)
				create_client();
			else
				handle_cmds(i);
			//std::cout << "nexte event " << i << std::endl;
		}
	}
}

Serveur::~Serveur()
{
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