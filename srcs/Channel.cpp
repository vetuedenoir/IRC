#include "Channel.hpp"

Channel::Channel(std::string &name, Client *founder) : _name(name)
{
	_founder = founder->getNickname();
	_my_clients[rcasemape(_founder)] = founder;
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
	_mode |= mode;
}

void	Channel::removeMode(ChanMode &mode)
{

}

bool	Channel::isModeSet(ChanMode &mode)
{

}

bool	Channel::sendMsg_toCli(std::string &msg)
{

}

int		Channel::getClientRights(std::string &nick)
{

}

bool	Channel::setClientRights(int rights)
{

}