#include "Client.hpp"

Client::Client(int sock_fd, struct sockaddr_in addr) : _sock_fd(sock_fd)
{
	struct hostent *host;
	in_addr_t ad;
	socklen_t			addrlen = sizeof(struct sockaddr);
	std::string			ip;

	_nickname = "_?_";
	_adresse_ip = inet_ntoa(addr.sin_addr);
	std::cout << "adresse ip >>>> " << _adresse_ip << std::endl;
	ad = inet_addr(_adresse_ip.c_str());
	host = gethostbyaddr((const char *)&ad, sizeof(struct in_addr), AF_INET);
	if (host == NULL)
		_host_cli = _adresse_ip;
	else
		_host_cli = host->h_name;
	std::cout << "hostname du client >>> " << _host_cli << std::endl;
	memset(&addr, 0, addrlen);
	if (getsockname(_sock_fd, (struct sockaddr *)&addr, &addrlen))
	 	run_error("Error: getsockname");
	
	ip = inet_ntoa(addr.sin_addr);

	ad = inet_addr(ip.c_str());
	host = gethostbyaddr((const char *)&ad, sizeof(struct in_addr), AF_INET);
	if (host == NULL)
		_host_serv = _adresse_ip;
	else
		_host_serv = host->h_name;


	std::time(&_time_connection);
	_time_last_msg = _time_connection;
	_is_auth = false;
}

Client::~Client()
{
	if (_sock_fd)
		close(_sock_fd);
}

const std::string &Client::getUsername() const
{
	return _username;
}

std::string &Client::getNickname()
{
	return _nickname;
}

const int &Client::getSock_fd() const
{
	return _sock_fd;
}

const time_t &Client::getTime_connection() const
{
	return _time_connection;
}

const time_t &Client::getTime_last_msg() const
{
	return _time_last_msg;
}

const std::string &Client::getAdresse_ip() const
{
	return _adresse_ip;
}

const std::string &Client::getInput_buf() const
{
	return _input_buf;
}

const int &Client::getIs_auth() const
{
	return _is_auth;
}

const std::string &Client::getHost_cli() const
{
	return _host_cli;
}

const std::string &Client::getHost_serv() const
{
	return _host_serv;
}

size_t	Client::getSizeBuff() const
{
	return _input_buf.size();
}

int Client::setInput_buf(std::string buff)
{
	_input_buf = buff;
	if (_input_buf[_input_buf.size() - 1] == '\n' || _input_buf[_input_buf.size() - 1] == '\r')
		return 0;
	else
		return 1;
}

void Client::setIs_auth(int level)
{
	_is_auth = level;
}

void Client::setNick(std::string &nick)
{
	_nickname = nick;
}

void	Client::setUsername(std::string nick)
{
	_username = nick;
}

int Client::verif_return()
{
	if (_input_buf.find('\n') != std::string::npos || _input_buf.find('\r') != std::string::npos)
		return 1;
	else
		return 0;
}

void Client::clearBuf()
{
	_input_buf = "";
}

int Client::sendMsg(const std::string &msg)
{
	if (send(_sock_fd, msg.c_str(), msg.size(), MSG_DONTWAIT) == -1)
		return (1);
	return (0);
}