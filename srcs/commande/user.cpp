#include "commande.hpp"

bool	user(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	(void)serveur;
	if (client->getIs_auth() < 2)
		return (0);
	else if (client->getIs_auth() == 3)
	{
		{
		if (client->sendMsg(ERR_ALREADYREGISTERED(client->getHost_serv())) == -1)
			return (1);
		return (0);
	}
	}
	if (arguments.size() < 4)
	{
		if (client->sendMsg(ERR_NEEDMOREPARAMS(client->getHost_serv(), "USER")) == -1)
			return (1);
		return (0);
	}
	if (arguments[0].size() < 1)
	{
		if (client->sendMsg(ERR_NEEDMOREPARAMS(client->getHost_serv(), "USER")) == -1)
			return (1);
		return (0);
	}
	client->setUsername(std::string("~") + arguments[0].substr(0, USERLEN));
	client->setIs_auth(3);
	client->sendMsg(RPL_WELCOM(client->getNickname()));
	client->setFullName();
	return 0;
}
