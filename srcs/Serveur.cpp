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
	//std::memset(&_sock_addr, 0, sizeof(struct sockaddr_in));
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
	struct tm *ptm;

	create_socket();
	bind_socket();
	set_commande();
	std::time(&_raw_time);
	ptm = localtime(&_raw_time);
	_str_time = asctime(ptm);
	std::cout << "debut du serveurt a " << _str_time << std::endl;
	if (listen(_socket_fd, SIZE_QUEUE) == 1)
		throw std::runtime_error("Error: Cannot listen to socket");
	create_epoll();
}

void	Serveur::create_client()
{
	int					client_fd;
	struct sockaddr_in	cli_sock_addr;
	socklen_t			addrlen;

	addrlen = sizeof(cli_sock_addr);
	client_fd = accept(_socket_fd, (struct sockaddr *)&cli_sock_addr, &addrlen);
	if (client_fd == -1)
		return (run_error("Cannot accept new connection: "));
	// if (getsockname(client_fd, (struct sockaddr *)&cli_sock_addr, &addrlen))
	// 	return (run_error("Error: getsockname"));
	if (fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK) == -1)
		return (close(client_fd), run_error("Cannot accept new connection: "));
	_event.events = EPOLLIN;
	_event.data.fd = client_fd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &_event) == -1)
		return (close(client_fd), run_error("Error: epoll_ctl"));
	Client *client = new(std::nothrow) Client(client_fd, cli_sock_addr);
	if (client == NULL)
	{
		epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
		return (close(client_fd), run_error("Error: new Client"));
	}
	nb_client++;
	_list_clients.insert(std::pair<int, Client *>(client_fd, client));
}

void	Serveur::remove_client(int fd, std::string reason)
{
	std::cout << "in remove client" << std::endl;
	std::map<std::string, int> cliChannel = _list_clients[fd]->getMychannel();
	std::map<std::string, int>::iterator cli_it;
	std::map<std::string, Channel *>::iterator serv_it;
	std::string nick = _list_clients[fd]->getNickname();
	std::string cnick = rcasemape(nick);

	for (cli_it = cliChannel.begin(); cli_it != cliChannel.end(); cli_it++)
	{
		serv_it = _list_channel.find(cli_it->first);
		if (serv_it->second->remove_cli(nick, reason))
		{
			delete _list_channel[serv_it->first];
			_list_channel.erase(serv_it->first);
		}
	}
	for (serv_it = _list_channel.begin(); serv_it != _list_channel.end(); serv_it++)
	{
		if (serv_it->second->isInvited(cnick))
			serv_it->second->remove_invite(cnick);
	}
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
	while (buffer[pos] == ' ')
		pos++;
	debut = pos;
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
		while (buffer[pos] == ' ')
			pos++;
		debut = pos;
	}
	while (buffer[debut] == '\r' || buffer[debut] == '\n')
		debut++;
	casemape(msg.commande);
	return (msg);
}

void	print_message(message_t &msg)
{
	if (msg.commande == "")
		return ;
	std::cout << "-------message---------------------------------" << std::endl;
	//std::cout << msg.commande << "|" <<  std::endl;
	// for (size_t i = 0; i < msg.parametres.size(); i++)
	// 	std::cout << "x " << msg.parametres[i] << "|" << std::endl;
}

void	Serveur::handle_cmds(int i)
{
	char	buffer[BUFF_SIZE + 1];
	int		count;
	Client	*client = _list_clients[_events_list[i].data.fd];

	count = recv(client->getSock_fd(), buffer, BUFF_SIZE, MSG_DONTWAIT);
	if (count == -1)
		return (run_error("Cannot read in socket: "));
	if (count == 0)
		return (remove_client(client->getSock_fd(), "ERROR :connection interompue"));
	buffer[count] = 0;
	if (count == 512 && (buffer[BUFF_SIZE - 1] != '\r' && buffer[BUFF_SIZE - 1] != '\n'))
		client->sendMsg(ERR_INPUTOOLONG(client->getNickname()));
	else
	{
		if (client->getSizeBuff() + count > 512)
		{
			client->clearBuf();
			client->sendMsg(ERR_INPUTOOLONG(client->getNickname()));
			return ;
		}
		if (!client->setInput_buf(client->getInput_buf() + buffer))
		{
			const std::string input = client->getInput_buf();
			//std::cout << client->getInput_buf();
			size_t		posn = input.find('\n');
			size_t		posr = input.find('\r');
			size_t		debut = 0;
			message_t	msg;
			while (posn != std::string::npos || posr != std::string::npos)
			{
		 		msg = parse_buff(input, debut, posn, posr);
				if (msg.commande.size() == 0)
					break ;
				if (_list_cmd.find(msg.commande) == _list_cmd.end())
				{
					client->sendMsg(ERR_UNKNOWNCOMMAND(client->getHost_serv(), msg.commande));
					break ;
				}
				if (_list_cmd[msg.commande](this, client, msg.parametres) == 1)
					return ;
				posn = input.find('\n', debut);
				posr = input.find('\r', debut);
				print_message(msg);
			}
			client->clearBuf();
		}
	}
}

void	Serveur::run_serveur()
{
	int	num_events;
	int	i;

	while (g_run)
	{
		num_events = epoll_wait(_epoll_fd, _events_list, MAX_EVENTS, -1);
		if (num_events == -1)
		{
			if (!g_run)
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
	std::map<int, Client *>::iterator 			cli_it;
	std::map<std::string, Channel *>::iterator	chan_it;

	if (_socket_fd > 0)
		close(_socket_fd);
	for (cli_it = _list_clients.begin(); cli_it !=_list_clients.end(); cli_it++)
		delete cli_it->second;
	for (chan_it = _list_channel.begin(); chan_it !=_list_channel.end(); chan_it++)
		delete chan_it->second;
	if (_epoll_fd > 0)
		close(_epoll_fd);
}



const std::string&	Serveur::getPass() const
{
	return _password;
}

std::map<int, Client *>&	Serveur::getList_clients()
{
	return _list_clients;
}

std::map<std::string, Channel *> Serveur::getList_channel()
{
	return _list_channel;
}


void	Serveur::set_commande()
{
	_list_cmd["CAP"] = &cap;
	_list_cmd["ERROR"] = &error;
	_list_cmd["INVITE"] = &invite;
	_list_cmd["JOIN"] = &join;
	_list_cmd["KICK"] = &kick;
	_list_cmd["MODE"] = &mode;
	_list_cmd["NICK"] = &nick;
	_list_cmd["PART"] = &part;
	_list_cmd["PASS"] = &pass;
	_list_cmd["PING"] = &ping;
	_list_cmd["PRIVMSG"] = &privmsg;
	_list_cmd["QUIT"] = &quit;
	_list_cmd["TOPIC"] = &topic;
	_list_cmd["USER"] = &user;
}

void	Serveur::addChannel(Channel *chan)
{
	_list_channel[rcasemape(chan->getName())] = chan;
}

Channel*	Serveur::getChan(const std::string &chanName)
{
	std::map<std::string, Channel *>::iterator it;

	it = _list_channel.find(chanName);
	if (it == _list_channel.end())
		return NULL;
	return it->second;
}

void	Serveur::remove_channel(const std::string &chanName)
{
	std::map<std::string, Channel *>::iterator it;
	
	it = _list_channel.find(chanName);
	if (it != _list_channel.end())
	{
		delete it->second;
		_list_channel.erase(chanName);
	}
}

