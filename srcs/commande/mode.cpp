#include "commande.hpp"

std::string	RPL_CHANNELMODEIS(std::string nickname, Channel *channel)
{
	std::string modestring("+");
	std::string	msg(":");
	std::string	parametre;

	msg.append(SERVEUR_NAME);
	msg.append(" 324 " + nickname + " " + channel->getName() + " ");

	if (channel->isModeSet(INVITATION))
		modestring.push_back('i');
	if (channel->isModeSet(TOPIC))
		modestring.push_back('t');
	if (channel->isModeSet(KEY))
	{
		modestring.push_back('k');
		parametre.append(channel->getKey());
	}
	if (channel->isModeSet(LIMITE))
	{
		modestring.push_back('l');
		if (parametre.size() != 0)
			parametre.push_back(' ');
		parametre.append(std::to_string(channel->getLimite()));
	}
	
	msg.append(modestring);
	if (parametre.size() != 0)
		msg.append(" " + parametre);
	msg.append("/r/n");
	return (msg);
}

bool	mode(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	(void)serveur;
	(void)client;
	(void)arguments;

	if (client->getIs_auth() != COMPLET_AUTH)
		return (0);
	if (arguments.size() == 0)
		return (client->sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE")));
	if (arguments[0][0] != '#')
		return (0);
	Channel *channel = serveur->getChan(rcasemape(arguments[0]));
	if (!channel)
		return (client->sendMsg(ERR_NOSUCHCHANNEL(client->getNickname(), arguments[0])));
	if (arguments.size() < 2)
	{
		client->sendMsg(RPL_CHANNELMODEIS(client->getNickname(), channel)));
		return (client->sendMsg(RPL_CREATIONTIME()));
	}
	

	return (0);
}
