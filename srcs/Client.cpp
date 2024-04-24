#include "Client.hpp"

Client::Client(int sock_fd, struct sockaddr_in addr) : _sock_fd(sock_fd), _addr(addr)
{
	struct hostent *host;
	in_addr_t ad;

	_nickname = "_?_";
	_adresse_ip = inet_ntoa(_addr.sin_addr);
	ad = inet_addr(_adresse_ip.c_str());
	host = gethostbyaddr((const char *)&ad, sizeof(struct in_addr), AF_INET);
	if (host == NULL)
		_hostname = _adresse_ip;
	else
		_hostname = host->h_name;
	std::time(&_time_connection);
	_time_last_msg = _time_connection;
	_is_auth = false;
}

Client::~Client()
{
	close(_sock_fd);
}


const std::string&	Client::getUsername() const
{
	return _username;
}

const std::string&	Client::getNickname() const
{
	return _nickname;
}

const int&	Client::getSock_fd() const
{
	return _sock_fd;
}

const time_t&		Client::getTime_connection() const
{
	return _time_connection;
}

const time_t&		Client::getTime_last_msg() const
{
	return	_time_last_msg;
}

const std::string&	Client::getAdresse_ip() const
{
	return _adresse_ip;
}

const std::string&	Client::getInput_buf() const
{
	return _input_buf;
}

const int&	Client::getIs_auth() const
{
	return _is_auth;
}

const std::string&	Client::getHostname() const
{
	return _hostname;
}


int	Client::setInput_buf(std::string buff)
{
	_input_buf = buff;
	if (_input_buf[_input_buf.size() - 1] == '\n' || _input_buf[_input_buf.size() - 1] == '\r')
		return 0;
	else
		return 1;
}

void	Client::setIs_auth(int level)
{
	_is_auth = level;
}


int Client::verif_return()
{
	if (_input_buf.find('\n') != std::string::npos || _input_buf.find('\r') != std::string::npos)
		return 1;
	else
		return 0;
}

void	Client::clearBuf()
{
	_input_buf = "";
}

int	Client::sendMsg(const std::string &msg)
{
	return (send(_sock_fd, msg.c_str(), msg.size(),  MSG_DONTWAIT));
}