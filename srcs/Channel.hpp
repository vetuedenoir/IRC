#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Client.hpp"

enum class ChanMode {
	NONE = 0,
	INVITATION = 1 << 0,
	TOPIC = 1 << 1,
	CLE = 1 << 2,
	LIMITE = 1 << 3
};

class Channel
{
	std::string _name;
	std::string	_founder;
	
	std::map<std::string, Client *> _my_clients;
	ChanMode	_mode;

 public:
	Channel(std::string &name, Client *oper);
	~Channel();

	std::string&	getName();

	ChanMode&		getMode();
	void			setMode(ChanMode &mode);
	void			addMode(ChanMode &mode);
	void			removeMode(ChanMode &mode);
	bool			isModeSet(ChanMode &mode);

	bool			sendMsg_toCli(std::string &msg);

	int				getClientRights(std::string &nick);
	bool			setClientRights(int rights);

};
#endif