#include "commande.hpp"

bool	pass(Serveur &serveur, Client &client, std::vector<std::string> &arguments)
{
	if (client.getIs_auth() == 0)
	{
		if (arguments.size() == 0)
		{
			if (client.sendMsg(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS")) == -1)
				return 1;
			return 0;
		}
		if (serveur.getPass() == arguments[0])
		{
			client.setIs_auth(1);
			return 0;
		}
		else
		{
			client.sendMsg(ERR_PASSWDMISMATCH(client.getHostname()));
			serveur.remove_client(client.getSock_fd());
			return 1;
		}
	}
	else
	{
		if (client.sendMsg(ERR_ALREADYREGISTERED(client.getHostname())) == -1)
			return 1;
		return 0;
	}
}
