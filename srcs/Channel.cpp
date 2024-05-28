#include "Channel.hpp"

Channel::Channel(std::string &name, Client *founder) : _name(name)
{
	_founder.first = rcasemape(founder->getNickname());
	_founder.second = founder;
	_mode = NONE;
	std::time(&_creation_time);
}

Channel::Channel(std::string &name, Client *founder, std::string &key) :_name(name), _key(key)
{
	_founder.first = rcasemape(founder->getNickname());
	_founder.second = founder;
	_mode = KEY;
}

Channel::~Channel()
{

}

std::string&	Channel::getName()
{
	return _name;
}

ChanMode&		Channel::getMode()
{
	return _mode;
}

void	Channel::setMode(ChanMode &mode)
{
	_mode = mode;
}

void	Channel::addMode(ChanMode &mode)
{
	_mode = _mode | mode;
}

void	Channel::removeMode(ChanMode &mode)
{
	_mode = static_cast<ChanMode>(static_cast<int>(_mode) & ~static_cast<int>(mode));
}

bool	Channel::isModeSet(ChanMode mode)
{
	return (static_cast<int>(_mode) & static_cast<int>(mode)) != 0;
}

bool	Channel::isInvited(std::string nick)
{
	if (_invited.find(nick) == _invited.end())
		return (0);
	return (1);
}

bool	Channel::sendMsg_toCli(const std::string &msg)
{
	if (_founder.first != "")
		_founder.second->sendMsg(msg);
	std::map<std::string, Client *>::iterator it;
	for (it = _my_clients.begin(); it != _my_clients.end(); it++)
		it->second->sendMsg(msg);
	return (0);
}

bool	Channel::sendPrivMsg(std::string &msg, std::string sender)
{
	if (_founder.first != sender)
		_founder.second->sendMsg(msg);
	std::map<std::string, Client *>::iterator it;
	for (it = _my_clients.begin(); it != _my_clients.end(); it++)
	{
		if (it->first != sender)
			it->second->sendMsg(msg);
	}
	return (0);
}


int		Channel::getClientRights(std::string nick)
{
	if (nick == _founder.first)
		return 5;
	std::map<std::string, Client *>::iterator it =  _my_clients.find(nick);
	if (it == _my_clients.end())
		return (-1);
	return (it->second->getRights(rcasemape(_name)));
}

std::string&	Channel::getKey()
{
	return (_key);
}

size_t&		Channel::getLimite()
{
	return (_limite);
}


std::string&	Channel::getTopic()
{
	return (_topic);
}

time_t&	Channel::getCreationTime()
{
	return (_creation_time);
}

void	Channel::setClientRights(const std::string &nick, int rights)
{
	if (_founder.first == nick)
		return (_founder.second->setRights(rcasemape(_name), rights));
	std::map<std::string, Client *>::iterator it = _my_clients.find(nick);
	it->second->setRights(rcasemape(_name), rights);
}

void	Channel::setTopic(const std::string &topic)
{
	_topic = topic.substr(0, TOPICLEN);
}

ChanMode operator| (ChanMode lhs, ChanMode rhs)
{
	return static_cast<ChanMode>(static_cast<int>(lhs) | static_cast<int>(rhs));
}


void	Channel::create_nameReply(Client *client)
{
	int			size_msg;
	int			rights;
	std::string	name_list;
	std::string	nick;
	std::map<std::string, Client *>::iterator it;

	std::string msg(":");
	msg.append(SERVEUR_NAME);
	msg.append(" 357 " + client->getNickname() + " = " + _name + " :");
	size_msg = msg.size();
	if (_founder.first != "")
		name_list.append("~" + _founder.first);
	it = _my_clients.begin();
	while (1)
	{
			nick = it->second->getNickname();
			if (size_msg + name_list.size() + nick.size() > 508)
			{
				client->sendMsg(msg + name_list + "\r\n");
				name_list = "";
			}
			rights = it->second->getRights(rcasemape(_name));
			it++;
			name_list.append(" ");
			if (rights == OPER)
				name_list.append("@" + nick);
			else
				name_list.append(nick);
			if (it == _my_clients.end())
			{
				client->sendMsg(msg + name_list + "\r\n");
				break ;
			}
	}
	client->sendMsg(RPL_ENDOFNAMES(client->getNickname(), _name));
}

bool	Channel::accepte_new_user(Client *client, const std::string &key)
{
	if (isModeSet(INVITATION))
	{
		if (_invited.find(rcasemape(client->getNickname())) == _invited.end())
			return (client->sendMsg(ERR_INVITEONLYCHAN(client->getNickname(), _name)));
	}
	if (isModeSet(KEY))
	{
		if (_key != key)
			return (client->sendMsg(ERR_BADCHANNELKEY(client->getNickname(), _name)));
	}
	if (isModeSet(LIMITE))
	{
		if (_limite == _my_clients.size())
			return (client->sendMsg(ERR_CHANNELISFULL(client->getNickname(), _name)));
	}
	_my_clients[rcasemape(client->getNickname())] = client;
	client->addChan(_name);
	setClientRights(rcasemape(client->getNickname()), NO);
	if (isModeSet(TOPIC))
		client->sendMsg(RPL_TOPIC(client->getNickname(), _name, _topic));
	std::string joinmsg(":");
	joinmsg += client->getFullName() + " JOIN :" + _name + "\r\n";
	sendMsg_toCli(joinmsg);
	if (_topic != "")
		client->sendMsg(RPL_TOPIC(client->getNickname(), _name, _topic));
	create_nameReply(client);
	return (0);
}

Client*			Channel::searchBigBoss()
{
	if (_founder.second != NULL)
		return _founder.second;
	std::map<std::string, Client *>::iterator	it;
	int			big_right = 0;
	std::string	casename = rcasemape(_name);

	for (it = _my_clients.begin(); it != _my_clients.end(); it++)
	{
		if (big_right < it->second->getRights(casename))
		{
			big_right = it->second->getRights(casename);
			if (big_right == OPER)
				return it->second;
		}
	}
	it = _my_clients.begin();
	return it->second;
}

bool	Channel::remove_cli(const std::string &nick, const std::string &reason)
{
	sendMsg_toCli(reason);


	std::cout << "in remove client to channel " << nick <<std::endl;
	if (_founder.first == rcasemape(nick))
	{
		_founder.second->quit_channel(rcasemape(_name));
		_founder.first = "";
		_founder.second = NULL;
	}
	else
	{
		Client *cli = _my_clients[rcasemape(nick)];
		cli->quit_channel(rcasemape(_name));
		_my_clients.erase(rcasemape(nick));
	}
	if (_founder.first == "" && _my_clients.size() == 0)
		return (1);
	return (0);
}

void	Channel::add_Invite(Client *client)
{
	_invited.insert(std::pair<std::string, Client *>(rcasemape(client->getNickname()), client));
}


void	Channel::remove_invite(const std::string &nick)
{
	_invited.erase(nick);
}
