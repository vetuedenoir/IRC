#include "commande.hpp"

std::string	RPL_CHANNELMODEIS(std::string nickname, Channel *channel)
{
	std::string modestring("+");
	std::string	msg(":");
	std::string	parametre;

	msg.append(SERVEUR_NAME);
	msg.append(" 324 " + nickname + " " + channel->getName() + " ");

	if (channel->isModeSet(INVITATION))
		modestring.push_back('i');
	if (channel->isModeSet(TOPIC))
		modestring.push_back('t');
	if (channel->isModeSet(KEY))
	{
		modestring.push_back('k');
		parametre.append("<key>");
	}
	if (channel->isModeSet(LIMITE))
	{
		modestring.push_back('l');
		if (parametre.size() != 0)
			parametre.push_back(' ');
		parametre.append(uIntToString(channel->getLimite()));
	}
	
	msg.append(modestring);
	if (parametre.size() != 0)
		msg.append(" " + parametre);
	msg.append("\r\n");
	return (msg);
}

std::string	remove_mode(Channel *channel, std::string &modestring, std::vector<std::string> arguments)
{
	std::vector<std::string>::iterator it_arg = arguments.begin();
	std::string			new_mode;

	new_mode.push_back(modestring[0]);
	for (size_t i = 1; i < modestring.size(); i++)
	{
		switch (modestring[i])
		{
			case 'i':
				if (channel->isModeSet(INVITATION))
				{
					channel->removeMode(INVITATION);
					new_mode.push_back(modestring[i]);
				}
				break ;
			case 't':
				if (channel->isModeSet(TOPIC))
				{
					channel->removeMode(TOPIC);
					new_mode.push_back(modestring[i]);
				}
				break ;
			case 'k':
				if (channel->isModeSet(KEY))
				{
					channel->removeMode(KEY);
					channel->setKey("");
					new_mode.push_back(modestring[i]);
				}
				break ;
			case 'o':
				if (channel->getClientRights(rcasemape(*it_arg)) == OPER)
				{
					channel->setClientRights(rcasemape(*it_arg), NONE);
					new_mode.push_back(modestring[i]);
				}
				break ;
			case 'l':
				if (channel->isModeSet(LIMITE))
				{
					channel->removeMode(LIMITE);
					channel->setLimite(UINT32_MAX);
					new_mode.push_back(modestring[i]);
				}
				break ;	
		}
	}
	if (new_mode.size() == 1)
		return ("");
	return (new_mode);
}

std::string	add_mode(Channel *channel, std::string &modestring, std::vector<std::string> arguments)
{
	std::vector<std::string>::iterator it_arg = arguments.begin();
	std::stringstream	ss;
	std::string			new_mode;
	std::string			new_arg;
	size_t				lim;

	new_mode.push_back(modestring[0]);
	for (size_t i = 1; i < modestring.size(); i++)
	{
		switch (modestring[i])
		{
			case 'i':
				if (!channel->isModeSet(INVITATION))
				{	
					channel->addMode(INVITATION);
					new_mode.push_back(modestring[i]);
				}
				break ;
			case 't':
				if (!channel->isModeSet(TOPIC))
				{
					channel->addMode(TOPIC);
					new_mode.push_back(modestring[i]);
				}
				break ;
			case 'k':
				if (it_arg == arguments.end())
						break ;
				if (!channel->isModeSet(KEY))
				{
					channel->addMode(KEY);
					channel->setKey((*it_arg).substr(0, KEYLEN));
					new_mode.push_back(modestring[i]);
					new_arg.append(" " + (*it_arg).substr(0, KEYLEN));
					it_arg++;
				}
				break ;
			case 'o':
				if (it_arg == arguments.end())
					break ;
				if (channel->getClientRights(rcasemape(*it_arg)) < OPER)
				{
					channel->setClientRights(rcasemape(*it_arg), OPER);
					new_mode.push_back(modestring[i]);
					new_arg.append(" " + *it_arg);
				}
				it_arg++;
				break ;
			case 'l':
				if (it_arg == arguments.end())
						break ;
				ss << *it_arg;
				if ((ss >> lim))
				{
					if (lim > 0 && lim != channel->getLimite())
					{
						channel->addMode(LIMITE);
						channel->setLimite(lim);
						new_mode.push_back(modestring[i]);
						new_arg.append(" " + *it_arg);
					}
				}
				it_arg++;
				break ;	
		}
	}
	if (new_mode.size() == 1)
		return ("");
	else
		new_mode.append(new_arg);
	return (new_mode);
}

bool	mode(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	std::string	new_mode;

	if (client->getIs_auth() != COMPLET_AUTH)
		return (0);
	if (arguments.size() == 0)
		return (client->sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE")));
	if (arguments[0][0] != '#')
		return (0);
	Channel *channel = serveur->getChan(rcasemape(arguments[0]));
	if (!channel)
		return (client->sendMsg(ERR_NOSUCHCHANNEL(client->getNickname(), arguments[0])));
	if (arguments.size() < 2)
	{
		client->sendMsg(RPL_CHANNELMODEIS(client->getNickname(), channel));
		return (client->sendMsg(RPL_CREATIONTIME(client->getNickname(), channel->getName(), \
			longToString(static_cast<long long>(channel->getCreationTime())))));
	}
	if (!channel->getClientRights(rcasemape(client->getNickname())))
		return (client->sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName())));
	
	if (arguments[1].size() == 1)
		return (0);
	if (arguments[1][0] == '+')
		new_mode = add_mode(channel, arguments[1], std::vector<std::string>(arguments.begin() + 2, arguments.end()));
	else if (arguments[1][0] == '-')
		new_mode = remove_mode(channel, arguments[1], std::vector<std::string>(arguments.begin() + 2, arguments.end()));
	else
		return (0);
	if (new_mode.size() == 0)
		return (0);
	
	std::string base_msg = ":";

	base_msg.append(client->getFullName());
	base_msg.append(" MODE ");
	base_msg.append(channel->getName());
	base_msg.append(" ");
	base_msg.append(new_mode + "\r\n");
	channel->sendMsg_toCli(base_msg);
	return (0);
}
