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
#include <iostream>

class Serveur
{
 private:

	int			port;
	std::string	password;
	int			socket_fd;


	void create_socket();
 public:
	Serveur(const int &port, const std::string &password_);
	~Serveur();
};

#endif