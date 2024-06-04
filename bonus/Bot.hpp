#pragma ONCE

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
#define PASS "Q"
#define PORT 8080
#define NICK "Bot"
#define USER "Bot Bot Bot Bot"
#define SIZE_BUFF 512

typedef struct message_s
{
	std::string 				source;
	std::string					commande;
	std::vector<std::string>	parametres;
}	message_t;

void send_command(int sockfd, const std::string cmd);