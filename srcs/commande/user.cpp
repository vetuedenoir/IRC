#include "commande.hpp"

bool	user(Serveur &serveur, Client &client, std::vector<std::string> &arguments)
{
	if (arguments.size() < 4)
	{
		if (client.sendMsg(ERR_NEEDMOREPARAMS(client.getHostname(), "USER")) == -1)
			return (1);
		return (0);
	}
}
