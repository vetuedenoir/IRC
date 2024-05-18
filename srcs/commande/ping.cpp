
#include "commande.hpp"

bool ping(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	(void)serveur;

	if (client->getIs_auth() != COMPLET_AUTH)
		return (0);
	if (arguments.size() == 0)
		return (0);
	return (client->sendMsg("PONG :" + arguments[0] + "\r\n"));
}