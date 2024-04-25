#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <string>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <iostream>
#include <cstring>

class Client
{
	std::string 		_username;
	std::string			_nickname;

	const int			_sock_fd;
	struct sockaddr_in	_addr;
	time_t				_time_connection;
	time_t				_time_last_msg;
	std::string			_adresse_ip;
	std::string			_hostname;
	int					_is_auth;

	std::string			_input_buf;

 public:
	Client(int sock_fd, struct sockaddr_in addr);
	~Client();

	const std::string&	getUsername() const;
	const std::string&	getNickname() const;
	const int&			getSock_fd() const;
	const time_t&		getTime_connection() const;
	const time_t&		getTime_last_msg() const;
	const std::string&	getAdresse_ip() const;
	const std::string&	getInput_buf() const;
	const int&			getIs_auth() const;
	const std::string&	getHostname() const;

	int	setInput_buf(std::string buff);
	void	setIs_auth(int level);
	void	setNick(std::string &nick);
	int verif_return();
	void	clearBuf();
	int		sendMsg(const std::string &msg);

	static size_t nb_client;


};

#endif