#ifndef SERVEUR_HPP
# define SERVEUR_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <cerrno>


#include <ctime>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>

#include "Client.hpp"
#include "Channel.hpp"


#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern bool	g_run;

class Client;
class Channel;

#define MAX_EVENTS 200
#define SIZE_QUEUE 100

void	run_error(std::string msg);

class Serveur
{
 private:

	time_t				_date_lancement;
	uint16_t			_port;
	std::string			_password;
	int					_socket_fd;
	struct sockaddr_in	_sock_addr;

	int					_epoll_fd;
	struct epoll_event	_event;
	struct epoll_event	_events_list[MAX_EVENTS];

	//Chaque client est identifier par son fd
	std::map<int, Client *> _list_clients;
	

	void	create_socket();
	void	bind_socket();
	void	create_epoll();
	void	create_client();
	void	handle_cmds(int i);
	void	remove_client(int fd);

 public:
	// long	;
	Serveur(const int &port, const std::string &password_);
	~Serveur();
	void	run_serveur();
};

#endif