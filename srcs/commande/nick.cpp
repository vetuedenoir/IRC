#include "commande.hpp"

bool	nick(Serveur &serveur, Client &client, std::vector<std::string> &arguments)
{
	if (arguments.size() == 0)
	{
		if (client.sendMsg(ERR_NONICKNAMEGIVEN(client.getHostname())) == -1)
			return 1;
		return 0;
	}
	else
	{
		if (std::isdigit(arguments[0][0]))
		{
			
		}
	}

}
