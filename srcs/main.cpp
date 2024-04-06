#include "irc.hpp"

int	main(int ac, char **av)
{
	if (ac != 3)
	{	
		std::cerr << "Error: Utilisation: ircserv <port <password>" << std::endl;
		return (1);
	}
	try
	{
		Serveur	serveur(av[1], av[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	
}