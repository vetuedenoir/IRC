#ifndef NUMERICS_HPP
#define NUMERICS_HPP

#define SERVEUR_NAME "Zen_Serve" 

#include "Serveur.hpp"
#include "Client.hpp"

#define CHANLIMIT	10
#define NICKLEN		15
#define USERLEN		15
#define	CHANNELLEN	64
#define	TOPICLEN	300
#define	PARTLEN		300
#define	KICKLEN		300
#define	AWAYLEN		300
#define	KEYLEN		32


#define NICK_CHARACTERS std::string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}[]'\'|")


#define RPL_WELCOM(nickname) (std::string(":") + SERVEUR_NAME + " 001 " + nickname + " :Welcome to the IRC " + SERVEUR_NAME +\
		", my dear " + nickname + " !!!" + "\r\n")

#define	RPL_YOURHOST(nickname) (std::string(":") + SERVEUR_NAME + " 002 " + nickname + " :Your host is " + SERVEUR_NAME + " version 1.0\r\n")

#define	RPL_CREATED(nickname, datetime) (std::string(":") + SERVEUR_NAME + " 003 " + nickname + "this server was created " + datetime + "\r\n")

#define RPL_ISUPPORT(nickname) (std::string(":") + SERVEUR_NAME + " 005 " + nickname + " CASEMAPPING=ascii CHANTYPES=# CHANLIMIT=#:10 CHANMODES=itkol NICKLEN=15 USERLEN=15 TOPICLEN=300 PARTLEN=300 KICKLEN=300 AWAYLEN=300 KEYLEN=32\r\n")

#define	RPL_CREATIONTIME(nickname, channel, creationtime) (std::string(":") + SERVEUR_NAME + " 329 " + nickname + " " + channel + " " + creationtime + "\r\n")

#define	RPL_TOPIC(nickname, channel, topic) (std::string(":") + SERVEUR_NAME + " 332 " + nickname + " " + channel + " :" + topic + "\r\n")

#define	RPL_INVITING(nickname, cible, channel) (std::string(":") + SERVEUR_NAME + " 341 " + nickname + " " + cible +  " " + channel + "\r\n")

#define RPL_NAMREPLY(nickname, symbol, channel, list) (std::string(":") + SERVEUR_NAME + " 353 " + nickname + " " + symbole + channel + " :" + list + "\r\n")

#define RPL_ENDOFNAMES(nickname, channel) (std::string(":") + SERVEUR_NAME + " 366 " + nickname + " " + channel + " :End of /NAMES list\r\n")

#define	ERR_NOSUCHNICK(nickname, cible) (std::string(":") + SERVEUR_NAME + " 401 " + cible + " :there is no nickame or channel named like this\r\n")

#define ERR_NOSUCHCHANNEL(nickname, channel) (std::string(":") + SERVEUR_NAME + " 403 " + nickname + " " + channel + " :No such channel\r\n")

#define ERR_CANNOTSENDTOCHAN(nickname, channel) (std::string(":") + SERVEUR_NAME + " 404 " + nickname + " " + channel + " :Cannot send to channel\r\n")

#define ERR_TOMANYCHANNEL(nickname, channel) (std::string(":") + SERVEUR_NAME + " 405 " + nickname + " " + channel + " :You have joined to many channel\r\n")

#define ERR_NOTEXTTOSEND() (std::string(":") + SERVEUR_NAME + " 412 :No text tos send\r\n")

#define ERR_INPUTOOLONG(nickname) (std::string(":") + SERVEUR_NAME + " 417 " + nickname + " Input line was too long\r\n")

#define ERR_UNKNOWNCOMMAND(host, commande) (std::string(":") + SERVEUR_NAME + " 421 " + host + " " + commande + " :Unknown commande\r\n")

#define ERR_NONICKNAMEGIVEN(host) (std::string(":") + SERVEUR_NAME + " 431 " + host + " :No nickname given\r\n")

#define ERR_ERRONEUSNICKNAME(host, nickname) (std::string(":") + SERVEUR_NAME + " 432 " + nickname + " :Erroneus nickname\r\n")

#define ERR_NICKNAMEINUSE(host, nickname) (std::string(":") + SERVEUR_NAME + " 433 " + host + " " + nickname + " :Nickname is already in use\r\n")

#define ERR_USERNOTINCHANNEL(nickname, cible, channel) (std::string(":") + SERVEUR_NAME + " 441 " + nickname + " " + cible +  " " + channel + " :this user is not in the channel\r\n")

#define ERR_NOTONCHANNEL(nickname, channel) (std::string(":") + SERVEUR_NAME + " 442 " + nickname + " " + channel + " :You're not in channel\r\n")

#define ERR_USERONCHANNEL(nickname, cible, channel) (std::string(":") + SERVEUR_NAME + " 443 " + nickname + " " + cible +  " " + channel + " :this user is already channel\r\n")

#define ERR_NEEDMOREPARAMS(nickname, command) (std::string(":") + SERVEUR_NAME + " 461 " + nickname + " " + command + " :Not enough parameters\r\n")

#define ERR_ALREADYREGISTERED(host) (std::string(":") + SERVEUR_NAME + " 462 " + host + " :You may not register\r\n")

#define ERR_PASSWDMISMATCH(host) (std::string(":") + SERVEUR_NAME + " 464 " + host + " :Password incorrect\r\n")

#define	ERR_CHANNELISFULL(nickname, channel) (std::string(":") + SERVEUR_NAME + " 471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n")

#define	ERR_INVITEONLYCHAN(nickname, channel) (std::string(":") + SERVEUR_NAME + " 473 " + nickname + " " + channel + " :Cannot join channel (+i)\r\n")

#define ERR_BADCHANNELKEY(nickname, channel) (std::string(":") + SERVEUR_NAME + " 475 " + nickname + " " + channel + " :Cannot join channel (+K)\r\n")

#define ERR_BADCHANMASK(channel) (std::string(":") + SERVEUR_NAME + " 476 " + channel + " :Bad Channel Mask\r\n")

#define ERR_CHANOPRIVSNEEDED(nickname, channel) (std::string(":") + SERVEUR_NAME + " 482 " + nickname + " " + channel + " :You're not channel operator\r\n") 


#endif