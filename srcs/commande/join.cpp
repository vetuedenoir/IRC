#include "commande.hpp"

std::vector<std::pair<std::string, std::string> >	parse(std::string &chan, std::string key)
{
	size_t	pos = 0;
	size_t	debut = 0;
	std::string	str;
	std::vector<std::pair<std::string, std::string> > chans_keys;

	while (pos != std::string::npos)
	{
		debut = pos;
		pos = chan.find(',', debut);
		str = chan.substr(debut, pos - debut);
		chans_keys.push_back(std::pair<std::string, std::string>(str, ""));
		while (chan[pos] == ',')
			pos++;
	}
	if (key.size() < 2)
		return chans_keys;
	pos = 0;
	size_t i = 0;
	while (pos != std::string::npos)
	{
		debut = pos;
		pos = key.find(',', debut);
		str = key.substr(debut, pos - debut);
		chans_keys[i].second = str;
		while (key[pos] == ',')
			pos++;
		i++;
	}
	return chans_keys;
}

bool	create_channel(Serveur *serveur, Client *client, std::pair<std::string, std::string> arg)
{
	if (client->getNumsChan() == CHANLIMIT)
	{
		client->sendMsg(ERR_TOMANYCHANNEL(client->getNickname(), arg.first));
		return (1);
	}
	Channel *channel = NULL;
	if (arg.second == "")
		channel = new(std::nothrow) Channel(arg.first, client);
	if (!channel)
	{
		std::string	msg(":");
		msg.append(SERVEUR_NAME);
		msg.append(" ERROR :Cannot create channel\r\n");
		client->sendMsg(msg);

		return (1);
	}
	serveur->addChannel(channel);
	client->addChan(channel->getName());
	channel->setClientRights(rcasemape(client->getNickname()), FOUNDER);
	std::string joinmsg(":");
	joinmsg += client->getFullName() + " JOIN :" + channel->getName() + "\r\n";
	channel->sendMsg_toCli(joinmsg);
	return (0);
}

bool	join(Serveur *serveur, Client *client, std::vector<std::string> &arguments)
{
	if (client->getIs_auth() != COMPLET_AUTH)
		return (0);
	if (arguments.size() == 0)
		return (client->sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN")));
	if (arguments[0] == "0")
		return (client->partAll(), 0);
	if (arguments[0].size() < 2)
		return (client->sendMsg(ERR_BADCHANMASK(arguments[0])));
	std::vector<std::pair<std::string, std::string> > liste;
	if (arguments.size() == 2)
		liste = parse(arguments[0], arguments[1]);
	else
		liste = parse(arguments[0], "");

	std::map<std::string, Channel *> list_channel = serveur->getList_channel();
	std::map<std::string, Channel *>::iterator it;
	std::string	chanName;
	for (size_t i = 0; i < liste.size(); i++)
	{
		chanName = rcasemape(liste[i].first);
		if (chanName.size() < 2 || chanName[0] != '#')
		{
			client->sendMsg(ERR_BADCHANMASK(liste[i].first));
			continue ;
		}
		it = list_channel.find(chanName);
		if (it == list_channel.end())
		{
			if (create_channel(serveur, client, liste[i]))
				break ;
		}
		else
		{
			if (client->inChan(chanName))
				continue ;
			if (client->getNumsChan() == 10)
				return (client->sendMsg(ERR_TOMANYCHANNEL(client->getNickname(), liste[i].first)));
			it->second->accepte_new_user(client, liste[i].second);			
		}
	}
	return (0);
}
