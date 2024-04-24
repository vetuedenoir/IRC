#ifndef NUMERICS_HPP
# define NUMERICS_HPP

# define SERVEUR_NAME "Zen Serve" 

#include "Serveur.hpp"
#include "Client.hpp"

#define NICK_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}[]'\'|"


#define RPL_WELCOM(nickname) (std::string(":") + SERVEUR_NAME + " 001 " + nickname + " :Welcome to the IRC " + SERVEUR_NAME + ", my dear " + nickname + " !!!" + "\r\n")

#define ERR_NONICKNAMEGIVEN(host) (std::string(":") + SERVEUR_NAME + " 431 " + host + " :No nickname given\r\n")

#define ERR_INPUTOOLONG(nickname) (std::string(":") + SERVEUR_NAME + " 417 " + nickname + "Input line was too long\r\n")

#define ERR_NEEDMOREPARAMS(nickname, command) (std::string(":") + SERVEUR_NAME + " 461 " + nickname + " " + command + " :Not enough parameters\r\n")

#define ERR_ALREADYREGISTERED(host) (std::string(":") + SERVEUR_NAME + " 462 " + host + " :You may not register\r\n")

#define ERR_PASSWDMISMATCH(host) (std::string(":") + SERVEUR_NAME + " 464 " + host + " :Password incorrect\r\n")


#endif