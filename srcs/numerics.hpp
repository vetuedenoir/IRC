#ifndef NUMERICS_HPP
#define NUMERICS_HPP

#define SERVEUR_NAME "Zen_Serve" 

#include "Serveur.hpp"
#include "Client.hpp"

#define CASEMAPPING "ascii"
#define CHANLIMIT	10
#define	CHANMODE	"CHANMODE= "
#define NICKLEN		15
#define USERLEN		15



#define NICK_CHARACTERS std::string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}[]'\'|")


#define RPL_WELCOM(nickname) (std::string(":") + SERVEUR_NAME + " 001 " + nickname + " :Welcome to the IRC " + SERVEUR_NAME +\
		", my dear " + nickname + " !!!" + "\r\n")


#define	ERR_NOSUCHNICK(nickname, cible) (std::string(":") + SERVEUR_NAME + " 401 " + cible + " :there is no nickame or channel named like this\r\n")

#define ERR_NOTEXTTOSEND() (std::string(":") + SERVEUR_NAME + " 412 :No text tos send\r\n")

#define ERR_INPUTOOLONG(nickname) (std::string(":") + SERVEUR_NAME + " 417 " + nickname + " Input line was too long\r\n")

#define ERR_UNKNOWNCOMMAND(host, commande) (std::string(":") + SERVEUR_NAME + " 421 " + host + " " + commande + " :Unknown commande\r\n")

#define ERR_NONICKNAMEGIVEN(host) (std::string(":") + SERVEUR_NAME + " 431 " + host + " :No nickname given\r\n")

#define ERR_ERRONEUSNICKNAME(host, nickname) (std::string(":") + SERVEUR_NAME + " 432 " + nickname + " :Erroneus nickname\r\n")

#define ERR_NICKNAMEINUSE(host, nickname) (std::string(":") + SERVEUR_NAME + " 433 " + host + " " + nickname + " :Nickname is already in use\r\n")

#define ERR_NEEDMOREPARAMS(nickname, command) (std::string(":") + SERVEUR_NAME + " 461 " + nickname + " " + command + " :Not enough parameters\r\n")

#define ERR_ALREADYREGISTERED(host) (std::string(":") + SERVEUR_NAME + " 462 " + host + " :You may not register\r\n")

#define ERR_PASSWDMISMATCH(host) (std::string(":") + SERVEUR_NAME + " 464 " + host + " :Password incorrect\r\n")


#endif