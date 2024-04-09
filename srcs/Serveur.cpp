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
	if (listen(_socket_fd, SOMAXCONN) == 1)
	{
		close(_socket_fd);
		throw std::runtime_error("Error: Cannot listen to socket");
	}
	create_epoll();
}

void	Serveur::run_serveur()
{

}

Serveur::~Serveur()
{
	if (_socket_fd > 0)
	{
		if (close(_socket_fd) == -1)
			throw std::runtime_error("Error: Cannot close socket fd");
	}
	//ferme les clients d habort;
	if (_epoll_fd > 0)
	{
		if (close(_epoll_fd) == -1)
			throw std::runtime_error("Error: Cannot close socket fd");
	}

	
}


