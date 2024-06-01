#ifndef CHANNEL_HPP
# define CHANNEL_HPP

enum ChanMode {
	NONE = 0,
	INVITATION = 1 << 0,
	TOPIC = 1 << 1,
	KEY = 1 << 2,
	LIMITE = 1 << 3
};

enum Rights {
	NO = 0,
	OPER = 3,
	FOUNDER = 5
};

ChanMode operator| (ChanMode lhs, ChanMode rhs);

#include "Client.hpp"
#include "numerics.hpp"

class Channel
{
	std::string _name;
	std::string	_key;
	std::string	_topic;
	size_t		_limite;

	time_t	_creation_time;

	std::pair<std::string, Client *> _founder;
	std::map<std::string, Client *> _my_clients;
	std::map<std::string, Client *> _invited;

	ChanMode	_mode;

 public:
	Channel(std::string name, Client *founder);
	~Channel();

	std::string&	getName();

	ChanMode&		getMode();
	void			setMode(ChanMode &mode);
	void			addMode(ChanMode mode);
	void			removeMode(ChanMode mode);
	bool			isModeSet(ChanMode mode);
	bool			isInvited(std::string nick);

	bool			sendMsg_toCli(const std::string &msg);
	bool			sendPrivMsg(std::string &msg, std::string sender);

	int				getClientRights(std::string nick);
	std::string&	getTopic();
	std::string&	getKey();
	size_t&			getLimite();
	time_t&			getCreationTime();

	void			setClientRights(const std::string &nick, int rights);
	void			setTopic(const std::string &topic);
	void			setKey(const std::string &key);
	void			setLimite(const size_t &limite);

	bool			accepte_new_user(Client *client, const std::string &key);
	void			add_Invite(Client *client);
	void			create_nameReply(Client *client);
	bool			remove_cli(const std::string &nick, const std::string &reason, bool option);
	void			remove_invite(const std::string &nick);
	void			change_nick(const std::string &old_nick, const std::string &new_nick);

};
#endif


