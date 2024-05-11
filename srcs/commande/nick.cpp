#include "commande.hpp"

bool	verif_occurence(std::string new_nick, std::map<int, Client *> &list_client)
{
	std::map<int, Client *>::iterator it;

	for (it = list_client.begin(); it != list_client.end(); it++)
	{
		if (new_nick == rcasemape(it->second->getNickname()))
			return (1);
	}
	return (0);
}

bool	nick(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	size_t i = 0;
	size_t len;

	if (!client->getIs_auth())
		return (0);
	if (arguments.size() == 0)
	{
		if (client->sendMsg(ERR_NONICKNAMEGIVEN(client->getHost_serv())) == -1)
			return (1);
		return (0);
	}
	len = arguments[0].size();
	if (len > NICKLEN)
	{
		if (client->sendMsg(ERR_ERRONEUSNICKNAME(client->getHostname(), arguments[0])) == -1)
			return (1);
		return (0);
	}
	if (std::isdigit(arguments[0][0]))
	{
		if (client->sendMsg(ERR_ERRONEUSNICKNAME(client->getHostname(), arguments[0])) == -1)
			return (1);
		return (0);
	}
	for (i = 0; i < len; i++)
	{
		if (NICK_CHARACTERS.find(arguments[0][i]) == std::string::npos)
		{
			if (client->sendMsg(ERR_ERRONEUSNICKNAME(client->getHostname(), arguments[0])) == -1)
				return (1);
			return (0);
		}
	}
	if (verif_occurence(rcasemape(arguments[0]), serveur->getList_clients()))
	{
		if (client->sendMsg(ERR_NICKNAMEINUSE(client->getHost_serv(), arguments[0])) == -1)
			return (1);
		return (0);
	}
	client->setNick(arguments[0]);
	if (client->getIs_auth() > 3)
	{
		std::string msg(":");
		msg = msg + SERVEUR_NAME + " new nickname " + arguments[0] + " sucesfully setup\r\n"; 
		if (client->sendMsg(msg) == -1)
			return (1);
	}
	else if (client->getIs_auth() == 1)
		client->setIs_auth(2);
	client->setFullName();
	return (0);
}
