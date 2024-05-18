#include "commande.hpp"


bool	kick(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	int	rights;
	std::string	base_msg;

	if (client->getIs_auth() != COMPLET_AUTH)
		return (0);
	if (arguments.size() < 2)
		return client->sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
	
	std::string cchan_name = rcasemape(arguments[0]);
	Channel *channel = serveur->getChan(cchan_name);

	if (channel == NULL)
		return (client->sendMsg(ERR_NOSUCHCHANNEL(client->getNickname(), arguments[0])));
	if (!client->inChan(cchan_name))
		return (client->sendMsg(ERR_NOTONCHANNEL(client->getNickname(), channel->getName())));
	if (channel->getClientRights(rcasemape(client->getNickname())) < OPER)
		return (client->sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName())));

	std::vector<std::string> list_user = split_virgule(arguments[1]);
	base_msg = ":";

	base_msg.append(client->getFullName());
	base_msg.append(" KICK ");
	base_msg.append(channel->getName());
	base_msg.append(" ");
	
	std::string	msg_perso;

	for (size_t i = 0; i < list_user.size(); i++)
	{
		rights = channel->getClientRights(rcasemape(list_user[i]));
		if (rights == -1)
			client->sendMsg(ERR_USERNOTINCHANNEL(client->getNickname(), list_user[i], channel->getName()));
		else if (rights <= OPER)
		{
			msg_perso = list_user[i];
			if (arguments.size() > 2)
				msg_perso.append(" :" + arguments[2]);
			else
				base_msg.append(" :" + client->getNickname());
			if (channel->remove_cli(rcasemape(list_user[i]), base_msg + msg_perso))
				serveur->remove_channel(cchan_name);
		}
	}
	return (0);
}


// faire un message de base ou je vais ajouter le nom des gens kicke , modifier a chaque tour
//declare des variable pour le nick et autre qui se repete