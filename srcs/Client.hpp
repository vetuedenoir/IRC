#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <string>

class Client
{
	std::string 		_username;

	const int			_sock_fd;
	struct sockaddr_in	_addr;
	time_t				_time_connection;
	time_t				_time_last_msg;
	std::string			_adresse_ip;
	bool				_connecte;

 public:
	Client(int sock_fd, struct sockaddr_in addr);
	~Client();

	std::string	getUsername() const;
	int			getSock_fd() const;
	time_t		getTime_connection() const;
	time_t		getTime_last_msg() const;
	std::string	getAdresse_ip() const;




};

#endif