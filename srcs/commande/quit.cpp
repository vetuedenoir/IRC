#include "commande.hpp"

bool	quit(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	std::string reason = "QUIT";
	if (arguments.size() >= 1)
		reason.append(" :" + arguments[0]);
	serveur->remove_client(client->getSock_fd(), reason);
	return (1);
}
