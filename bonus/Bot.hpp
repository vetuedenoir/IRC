#pragma once

#include <csignal>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <map>

#include "Joueur.hpp"

#define SERVER "localhost"
#define NICK "Bot"
#define USER "Bot Bot Bot Bot"
#define SIZE_BUFF 512


	typedef struct message_s
	{
		std::string 				source;
		std::string					commande;
		std::vector<std::string>	parametres;
	}	message_t;

class Joueur;

class Bot
{
	private :
		std::string						_pass;
		std::map<std::string, Joueur>	list_joueur;

	public :

		Bot(std::string pass);
		~Bot();
		message_t		parse_buff(const std::string &buffer);
		std::string		extracte_nick(std::string &source);
		int	 			is_digit(const char *buffer);
		void			print_message(message_t &msg);
		void 			loop_bot(int sockfd);
		void			process_command(message_t msg, int sockfd);
};
void			send_command(int sockfd, const std::string cmd);