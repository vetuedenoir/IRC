#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

#include <unistd.h>

class Client
{
	std::string _username;
	int	_sock_fd;
 public:
	Client(int sock_fd);
	~Client();


};

#endif