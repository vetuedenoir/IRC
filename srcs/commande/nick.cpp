#include "commande.hpp"

bool	nick(Serveur &serveur, Client &client, std::vector<std::string> &arguments)
{
	size_t i = 0;

	if (arguments.size() == 0)
	{
		if (client.sendMsg(ERR_NONICKNAMEGIVEN(client.getHostname())) == -1)
			return (1);
		return (0);
	}
	if (std::isdigit(arguments[0][0]))
	{
		if (client.sendMsg(ERR_ERRONEUSNICKNAME(client.getHostname(), arguments[0])) == -1)
			return (1);
		return (0);
	}
	for (i = 0; i < arguments[0].size(); i++)
	{
		if (NICK_CHARACTERS.find(arguments[0][i]) == std::string::npos)
		{
			if (client.sendMsg(ERR_ERRONEUSNICKNAME(client.getHostname(), arguments[0])) == -1)
				return (1);
			return (0);
		}
	}
	std::map<int, Client *> list_client = serveur.getList_clients();
	for (i = 0; i < list_client.size(); i++)
	{
		if (arguments[0] == list_client[i]->getNickname())
		{
			if (list_client[i]->getSock_fd() != client.getSock_fd())
			{
				if (client.sendMsg(ERR_NICKNAMEINUSE(client.getHostname(), arguments[0])) == -1)
					return (1);
				return (0);
			}
		}
	}
	client.setNick(arguments[0]);
	if (client.getIs_auth() > 3)
	{
		std::string msg(":");
		msg = msg + SERVEUR_NAME + " new nickname " + arguments[0] + " sucesfully setup\r\n"; 
		if (client.sendMsg(msg) == -1)
			return (1);
	}
	return (0);
}
