#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

class Client
{
	std::string _username;
	int	_sock_fd;
 public:
	Client(int sock_fd);
	~Client();


};

#endif