#include "commande.hpp"

bool	user(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	(void)serveur;

	if (!client->getIs_auth())
		return (0);
	else if (!NOT_AUTH_USER(client->getIs_auth()))
		return  (client->sendMsg(ERR_ALREADYREGISTERED(client->getHost_serv())));
	if (arguments.size() < 4)
		return (client->sendMsg(ERR_NEEDMOREPARAMS(client->getHost_serv(), "USER")));
	if (arguments[0].size() < 1)
		return (client->sendMsg(ERR_NEEDMOREPARAMS(client->getHost_serv(), "USER")));
	client->setUsername(std::string("~") + arguments[0].substr(0, USERLEN));
	client->setIs_auth(client->getIs_auth() + PLUS_USER);
	if (client->getIs_auth() == COMPLET_AUTH)
		client->sendMsg(RPL_WELCOM(client->getNickname()));
	client->setFullName();
	return (0);
}
