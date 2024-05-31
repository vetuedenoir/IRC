#include "irc.hpp"
#include "Serveur.hpp"
#include <cerrno>

bool g_run = 1;

void	sigint_handler(int signo)
{
	(void)signo;
	g_run = 0;
}

void init_passw(const std::string &pass)
{
	// bool	requirement[4] = {0};

	if (pass.size() < 1)
	 	throw "Erreur MDP: trop court, minimum 6 caracteres";
	if (pass.size() > 505)
		throw "Erreur MDP: trop long, maximum 505 caracteres";
	char	c;
	for (size_t i = 0; i < pass.size(); i++)
	{
		c = pass[i];
		// if (isdigit(c))
		// 	requirement[0] = true;
		// if (islower(c))
		// 	requirement[1] = true;
		// if (isupper(c))
		// 	requirement[2] = true;
		// if (ispunct(c))
		// 	requirement[3] = true;
		if (!isgraph(c))
			throw "Erreur MDP: uniquement des caracteres graphique accepter";
	}
	// if (!requirement[0] || !requirement[1] || !requirement[2] || !requirement[3])
	// 	throw "Erreur MDP: doit comporter au moins une majuscule, une minuscule, un chiffre et un caractere speciales";
}

uint16_t	init_port(const std::string &strport)
{
	char	*endptr= NULL;
	int		port = 0;

	if (strport.size() > 5)
		throw "Erreur: port inexistant";
	port = std::strtol(strport.c_str(), &endptr, 10);
	if (endptr != &strport[strport.length()])
		throw "Erreur: port inexistant";
	if (port < 0 || port > 65535)
		throw "Erreur: port inexistant";
	if (port == 0)
		throw	"Erreur: invalide port";
	return (port);
}

int	main(int ac, char **av)
{
	if (ac != 3)
	{	
		std::cerr << "Error: Utilisation: ircserv <port <password>" << std::endl;
		return (1);
	}
	errno = 0;
	std::signal(SIGINT, &sigint_handler);
	std::signal(SIGQUIT, &sigint_handler);

	try
	{
		init_passw(av[2]);	
		Serveur serveur(init_port(av[1]), av[2]);
		serveur.run_serveur();
	}
	catch(const std::exception& e) {
		std::cerr << e.what();
		if (errno != 0)
			std::cerr << ": " <<  std::strerror(errno);
		std::cerr<< std::endl;
		return (1);
	}
	catch(const char * e) {
		std::cerr << e << '\n';
		return (1);
	}	
	return (0);
}