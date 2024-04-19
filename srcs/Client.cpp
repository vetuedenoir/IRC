#include "Client.hpp"

Client::Client(int sock_fd, struct sockaddr_in addr) : _sock_fd(sock_fd), _addr(addr)
{
	_adresse_ip = inet_ntoa(_addr.sin_addr);
	std::time(&_time_connection);
	_time_last_msg = _time_connection;
	_connecte = false;
}

Client::~Client()
{
	close(_sock_fd);
}


std::string	Client::getUsername() const
{
	return _username;
}

int			Client::getSock_fd() const
{
	return _sock_fd;
}

time_t		Client::getTime_connection() const
{
	return _time_connection;
}

time_t		Client::getTime_last_msg() const
{
	return	_time_last_msg;
}

std::string	Client::getAdresse_ip() const
{
	return _adresse_ip;
}