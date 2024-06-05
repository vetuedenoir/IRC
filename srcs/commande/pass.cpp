#include "commande.hpp"

bool	pass(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	if (client->getIs_auth() == 0)
	{
		if (arguments.size() == 0)
			return (client->sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "PASS")));
		if (serveur->getPass() == arguments[0])
		{
			client->setIs_auth(1);
			return (0);
		}
		else
		{
			client->sendMsg(ERR_PASSWDMISMATCH(client->getHost_serv()));
			serveur->remove_client(client->getSock_fd(), "ERROR :Wrong password");
			return (1);
		}
	}
	else
		return (client->sendMsg(ERR_ALREADYREGISTERED(client->getHost_serv())));
}
