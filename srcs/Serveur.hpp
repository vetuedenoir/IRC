#ifndef SERVEUR_HPP
# define SERVEUR_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <cstring>
#include <iostream>

class Serveur
{
 private:

	int					_port;
	std::string			_password;
	int					_socket_fd;
	struct sockaddr_in	_sock_addr;

	int					_epoll_fd;
	struct epoll_event	_event;
	


	void	create_socket();
	void	bind_socket();
	void	create_epoll();
 public:
	Serveur(const int &port, const std::string &password_);
	~Serveur();
};

#endif