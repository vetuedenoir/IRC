#ifndef COMMANDE_HPP
# define COMMANDE_HPP

#include "../Serveur.hpp"

class Client;
class Serveur;


bool	cap(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	error(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	invite(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	join(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	kick(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	mode(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	nick(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	part(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	pass(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	privmsg(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	quit(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	topic(Serveur *serveur, Client *client, std::vector<std::string> &arguments);
bool	user(Serveur *serveur, Client *client, std::vector<std::string> &arguments);


#endif