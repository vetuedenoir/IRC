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

#include "utils.hpp"
#include "numerics.hpp"
#include "commande/commande.hpp"


#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern bool	g_run;

class Client;
class Channel;

#define MAX_EVENTS	200
#define SIZE_QUEUE	100
#define	BUFF_SIZE	512

typedef struct message_s
{
	// std::string 				source;
	std::string					commande;
	std::vector<std::string>	parametres;
}	message_t;


class Serveur
{
 private:

	time_t				_raw_time;
	std::string			_str_time;
	uint16_t			_port;
	std::string			_password;
	int					_socket_fd;
	struct sockaddr_in	_sock_addr;

	int					_epoll_fd;
	struct epoll_event	_event;
	struct epoll_event	_events_list[MAX_EVENTS];

	//Chaque client est identifier par son fd
	std::map<int, Client *> _list_clients;
	std::map<std::string, Channel *> _list_channel;

	std::map<std::string, bool (*)(Serveur *, Client *, std::vector<std::__cxx11::basic_string<char> >&)> _list_cmd;

	

	void	create_socket();
	void	bind_socket();
	void	create_epoll();
	void	create_client();
	void	handle_cmds(int i);
	message_t	parse_buff(const std::string &buffer, size_t &debut, size_t posn, size_t posr);
	void	set_commande();

 public:
	// long	;
	Serveur(const int &port, const std::string &password_);
	~Serveur();
	void	run_serveur();

	const std::string& getPass() const;
	std::map<int, Client *>&	getList_clients();
	std::map<std::string, Channel *> getList_channel();

	void	addChannel(Channel *chan);
	Channel*	getChan(const std::string &chanName);

	void	remove_client(int fd, std::string reason);
	void	remove_channel(const std::string &chanName);
};

#endif