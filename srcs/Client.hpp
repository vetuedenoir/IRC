#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

#include <unistd.h>

class Client
{
	std::string 		_username;
	int					_sock_fd;
	// bool				_connecte;
	//struct sockaddr_in	_addr;
 public:
	Client(int sock_fd);
	~Client();


};

#endif