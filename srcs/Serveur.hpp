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
#include <vector>

#include "Client.hpp"
#include "Channel.hpp"

extern bool	g_run;

class Client;
class Channel;

void	run_error(char *str);

class Serveur
{
 private:

	uint16_t			_port;
	std::string			_password;
	int					_socket_fd;
	struct sockaddr_in	_sock_addr;

	int					_epoll_fd;
	struct epoll_event	_events[201];

	std::vector<Client *> _list_clients;
	

	void	create_socket();
	void	bind_socket();
	void	create_epoll();
	void	create_client();
	void	handle_cmds(int i);
 public:
	Serveur(const int &port, const std::string &password_);
	~Serveur();
	void	run_serveur();
};

#endif