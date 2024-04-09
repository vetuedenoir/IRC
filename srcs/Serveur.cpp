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
	{
		close(_socket_fd);
		throw std::runtime_error("Error: Cannot set socket");
	}
}



void	Serveur::bind_socket()
{
	std::memset(&_sock_addr, 0, sizeof(sockaddr_in));
	_sock_addr.sin_family = AF_INET;
	_sock_addr.sin_addr.s_addr = INADDR_ANY;
    _sock_addr.sin_port = htons(_port);
	if (bind(_socket_fd, (struct sockaddr *)&_sock_addr, sizeof(_sock_addr) == -1))
	{
		close(_socket_fd);
		throw std::runtime_error("Error: Cannot bind socket");
	}
}

void	Serveur::create_epoll()
{
	
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

Serveur::~Serveur()
{

}