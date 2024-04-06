#ifndef SERVEUR_HPP
# define SERVEUR_HPP

#include <sys/socket.h>
#include <string>
#include <iostream>

class Serveur
{
 private:

	int	port;
	std::string	password;

	Serveur();
	Serveur(const Serveur &copie);
	Serveur& operator= (const Serveur &model);
 public:
	Serveur(const std::string &port, const std::string &password_);
	~Serveur();
};

#endif