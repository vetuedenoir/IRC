
#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ctime>
#include <iostream>
#include <cstring>
#include <string>
#include <map>

#include "utils.hpp"

#define	AUTH_NICK(level) (level != 1 && level != 5)

#define	AUTH_USER(level) (level != 1 && level != 3)

#define PLUS_PASS 1
#define	PLUS_NICK 2
#define	PLUS_USER 4
#define	COMPLET_AUTH	7

class Client
{
	std::string 		_username;
	std::string			_nickname;
	std::string			_fullName;

	const int			_sock_fd;
	std::string			_adresse_ip;
	std::string			_host_cli;
	std::string			_host_serv;
	int					_is_auth;

	std::string			_input_buf;

	std::map<std::string, int> _my_channel;

 public:
	Client(int sock_fd, struct sockaddr_in addr);
	~Client();

	const std::string&	getUsername() const;
	std::string&		getNickname();
	const int&			getSock_fd() const;
	const time_t&		getTime_connection() const;
	const time_t&		getTime_last_msg() const;
	const std::string&	getAdresse_ip() const;
	const std::string&	getInput_buf() const;
	const int&			getIs_auth() const;
	const std::string&	getHost_cli() const;
	const std::string&	getHost_serv() const;
	int					getRights(std::string chanName);
	std::string&		getFullName();
	size_t				getSizeBuff() const;		

	std::map<std::string, int>&	getMychannel();


	int		setInput_buf(std::string buff);
	void	setIs_auth(int level);
	void	setNick(std::string &nick);
	void	setUsername(std::string nick);
	void	setRights(std::string chanName, int rights);
	void	setFullName();

	int 	verif_return();
	void	clearBuf();
	int		sendMsg(const std::string &msg);
	int		getNumsChan();

	void	addChan(std::string &chan);
	bool	inChan(const std::string &chan);
	void	quit_channel(std::string chanName);

};

#endif
