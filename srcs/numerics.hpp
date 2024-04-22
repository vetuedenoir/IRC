#ifndef NUMERICS_HPP
# define NUMERICS_HPP

# define SERVEUR_NAME "Zen Serve" 

#include "Serveur.hpp"
#include "Client.hpp"


#define RPL_WELCOM(nickname) (std::string(":") + SERVEUR_NAME + " 001 " + nickname + " :Welcome to the IRC " + SERVEUR_NAME + ", my dear " + nickname + " !!!" + "\r\n")

#define ERR_INPUTOOLONG(nickname) (std::string(":") + SERVEUR_NAME + " 417 " + nickname + "Input line was too long\r\n")
#endif