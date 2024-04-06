#include "Serveur.hpp"

Serveur::Serveur() {};

Serveur::Serveur(const Serveur &copie) {(void)copie;}

Serveur& Serveur::operator= (const Serveur &model) {(void)model; return *this;}

std::string init_passw(const std::string &pass)
{
	bool	requirement[3] = {0};

	if (pass.size() < 6)
		throw "Erreur MDP: trop court, minimum 6 caracteres";
	if (pass.size() > 24)
		throw "Erreur MDP: trop long, maximum 26 caracteres";
	char	c;
	for (int i = 0; i < pass.size(); i++)
	{
		c = pass[i];
		if (isdigit(c))
			requirement[0] = true;
		if (isalpha(c))
			requirement[1] = true;
		if (isupper(c))
			requirement[2] = true;
		if (!isalnum(c))
			throw "Erreur MDP: uniquement des caracteres alphanumerique accepter";
	}
	if (!requirement[0] || !requirement[1] || !requirement[2])
		throw "Erreur MDP: doit comporter au moins une majuscule, une minuscule et un chiffre";
	return (pass);
}

int	init_port(const std::string &strport)
{
	char	*endptr= NULL;
	int		port = 0;

	if (strport.size() > 5)
		throw "Erreur: port inexistant";
	port = (int)strtod(strport.c_str(), &endptr);
	if (port <= 0)
		throw "Erreur: port inexistant";
	if (endptr != &strport[strport.length()])
		throw "Erreur: port inexistant";
	return port;
}

Serveur::Serveur(const std::string &port_, const std::string &password_)
{
	try
	{
		password = init_passw(password_);
		port = init_port(port_)
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
}

Serveur::~Serveur()
{

}