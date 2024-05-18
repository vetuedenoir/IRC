#include "commande.hpp"

bool	part(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	std::string	rchan;
	std::string	msg;
	Channel 	*channel;
	std::string	nickname = client->getNickname();

	if (client->getIs_auth() != COMPLET_AUTH)
		return (0);
	if (arguments.size() < 1)
		return (client->sendMsg(ERR_NEEDMOREPARAMS(nickname, "PART")));
	std::vector<std::string> listOfChan = split_virgule(arguments[0]);	

	std::vector<std::string>::iterator it;
	for (it = listOfChan.begin(); it != listOfChan.end(); it++)
	{
		rchan = rcasemape(*it);
		channel = serveur->getChan(rchan);
		if (!channel)
		{
			client->sendMsg(ERR_NOSUCHCHANNEL(nickname, *it));
			continue ;
		}
		if (!client->inChan(rchan))
		{
			client->sendMsg(ERR_NOTONCHANNEL(nickname, *it));
			continue ;
		}
		client->quit_channel(rchan);
		if (arguments.size() > 1)
			msg.append(client->getFullName() + " PART " + *it + " :" + arguments[1] + "\r\n");
		else
			msg.append(client->getFullName() + " PART :" + *it + "\r\n");
	}

	return (0);
}
