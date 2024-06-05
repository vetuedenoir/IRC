#include "commande.hpp"

Client *verif_occ(std::string cible, std::map<int, Client *> &list_client)
{
	std::map<int, Client *>::iterator it;

	for (it = list_client.begin(); it != list_client.end(); it++)
	{
		if (cible == rcasemape(it->second->getNickname()))
			return (it->second);
	}
	return (NULL);
}

bool privmsg(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	std::string cible;
	std::string msg;

	if (client->getIs_auth() != COMPLET_AUTH)
		return (0);
	if (arguments.size() == 0)
		return (client->sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG")));
	if (arguments.size() == 1)
		return (client->sendMsg(ERR_NOTEXTTOSEND()));
	cible = arguments[0].substr(0, arguments[0].find(','));
	if (cible[0] != '#')
	{
		Client *client_cible = verif_occ(rcasemape(cible), serveur->getList_clients());
		if (client_cible == NULL)
			return (client->sendMsg(ERR_NOSUCHNICK(client->getNickname(), cible)));
		msg = ":" + client->getNickname() + "!" + client->getUsername() + "@" + SERVEUR_NAME;
		msg.append(" PRIVMSG " + cible + " :");
		msg.append(arguments[1]);
		msg.append("\r\n");
		client_cible->sendMsg(msg);
	}
	else
	{
		Channel *channel = serveur->getChan(rcasemape(cible));
		if (!channel)
			return (client->sendMsg(ERR_CANNOTSENDTOCHAN(client->getNickname(), cible)));
		else if (channel->getClientRights(rcasemape(client->getNickname())) == -1)
			return (client->sendMsg(ERR_CANNOTSENDTOCHAN(client->getNickname(), cible)));
		msg = ":" + client->getFullName();
		msg.append(" PRIVMSG " + cible + " :");
		msg.append(arguments[1]);
		msg.append("\r\n");
		channel->sendPrivMsg(msg, rcasemape(client->getNickname()));
	}
	return (0);
}
