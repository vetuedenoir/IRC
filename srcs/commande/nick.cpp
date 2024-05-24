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
		return (client->sendMsg(ERR_NONICKNAMEGIVEN(client->getHost_serv())));
	len = arguments[0].size();
	if (len > NICKLEN)
		return (client->sendMsg(ERR_ERRONEUSNICKNAME(client->getHostname(), arguments[0])));
	if (std::isdigit(arguments[0][0]))
		return (client->sendMsg(ERR_ERRONEUSNICKNAME(client->getHostname(), arguments[0])));
	for (i = 0; i < len; i++)
	{                                                                                                        
		if (NICK_CHARACTERS.find(arguments[0][i]) == std::string::npos)
			return (client->sendMsg(ERR_ERRONEUSNICKNAME(client->getHostname(), arguments[0])));
	}
	if (verif_occurence(rcasemape(arguments[0]), serveur->getList_clients()))
		return (client->sendMsg(ERR_NICKNAMEINUSE(client->getHost_serv(), arguments[0])));
	if (client->getIs_auth() == COMPLET_AUTH)
	{
		std::string msg(":");
		msg += client->getFullName() + " NICK :" + arguments[0] + "\r\n";
		return (client->sendMsg(msg));
	}
	else if (NOT_AUTH_NICK(client->getIs_auth()))
		client->setIs_auth(client->getIs_auth() + PLUS_NICK);
	client->setNick(arguments[0]);
	client->setFullName();
	return (0);
}
