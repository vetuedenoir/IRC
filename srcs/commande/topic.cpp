#include "commande.hpp"

bool	topic(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	int	rights;

	if (client->getIs_auth() < 3)
		return (0);
	if (arguments.size() < 2)
		return (client->sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC")));
	
	std::string cchan_name = rcasemape(arguments[0]);
	Channel *channel = serveur->getChan(cchan_name);

	if (channel == NULL)
		return (client->sendMsg(ERR_NOSUCHCHANNEL(client->getNickname(), arguments[0])));
	if (!client->inChan(cchan_name))
		return (client->sendMsg(ERR_NOTONCHANNEL(client->getNickname(), channel->getName())));
	
	rights = client->getRights(cchan_name);
	if (channel->isModeSet(TOPIC))
	{
		if (rights >= OPER)
			channel->setTopic(arguments[1]);
		else
			return (client->sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName())));
	}
	else
		channel->setTopic(arguments[1]);
	std::string	msg;
	msg = ":" + client->getFullName();
	msg.append(" TOPIC " + channel->getName() + " :" + channel->getTopic() + "\r\n");
	channel->sendMsg_toCli(msg);
	return (0);
}
