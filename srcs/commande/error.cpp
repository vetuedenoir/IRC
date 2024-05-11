#include "commande.hpp"

bool	error(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	(void)serveur;
	std::string	msg("ERROR :");
	if (arguments.size() > 0)
		msg.append(arguments[0]);
	msg.append("\r\n");
	client->sendMsg(msg);
	return (0);
}