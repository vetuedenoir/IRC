#include "commande.hpp"

std::vector<std::string>	split_user(std::string &arg)
{
	size_t	pos = 0;
	size_t	debut = 0;
	std::string	str;
	std::vector<std::string> list_user;

	while (pos != std::string::npos)
	{
		debut = pos;
		pos = arg.find(',', debut);
		str = arg.substr(debut, pos - debut);
		list_user.push_back(str);
		while (arg[pos] == ',')
			pos++;
	}
	return (list_user);
}

bool	kick(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	int	rights;

	if (client->getIs_auth() < 3)
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

	std::vector<std::string> list_user = split_user(arguments[1]);
	std::string		reason("KICK ");

	if (arguments.size() > 2)
		reason.append(arguments[2]);
	else
		reason = "";
	for (size_t i = 0; i < list_user.size(); i++)
	{
		rights = channel->getClientRights(rcasemape(list_user[i]));
		if (rights == -1)
			client->sendMsg(ERR_USERNOTINCHANNEL(client->getNickname(), list_user[i], channel->getName()));
		else if (rights <= OPER)
		{
			if (channel->remove_cli(rcasemape(list_user[i]), reason))
				serveur->remove_channel(cchan_name);
		}
	}
	return (0);
}