#include "commande.hpp"

bool	invite(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{

	if (client->getIs_auth() != COMPLET_AUTH)
		return (0);
	if (arguments.size() < 2)
		return (client->sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE")));
	
	std::string cchan_name = rcasemape(arguments[1]);
	Channel *channel = serveur->getChan(cchan_name);

	if (!channel)
		return (client->sendMsg(ERR_NOSUCHCHANNEL(client->getNickname(), arguments[1])));
	if (!client->inChan(cchan_name))
		return (client->sendMsg(ERR_NOTONCHANNEL(client->getNickname(), channel->getName())));

	if (channel->isModeSet(INVITATION))
	{
		if (channel->getClientRights(rcasemape(client->getNickname())) < OPER)
			return (client->sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName())));	
	}

	Client *the_invite = serveur->getClientByName(rcasemape(arguments[0]));
	if (!the_invite)
		return (client->sendMsg(ERR_NOSUCHNICK(client->getNickname(), arguments[0])));
	if (the_invite->inChan(cchan_name))
		return (client->sendMsg(ERR_USERONCHANNEL(client->getNickname(), arguments[0], arguments[1])));
	
	channel->add_Invite(the_invite);
	client->sendMsg(RPL_INVITING(client->getNickname(), the_invite->getNickname(), channel->getName()));
	
	std::string	msg(":");
	msg.append(client->getFullName());
	msg.append(" INVITE ");
	msg.append(the_invite->getNickname());
	msg.append(" :");
	msg.append(channel->getName());
	msg.append("\r\n");
	the_invite->sendMsg(msg);

	return (0);
}