#include "Serveur.hpp"


//Creer la socket utilisant le protocol AF_INET(internet ipv4) de type stream (TCP/IP)
//on lui ajoute ensuite l option SO_REUSEADDR qui lui permet de reutiliser une adreese local

void	Serveur::create_socket()
{
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		throw std::runtime_error("Error: Cannot creat socket");
	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(socket_fd);
		throw std::runtime_error("Error: Cannot set socket");
	}
}

Serveur::Serveur(const int &port_, const std::string &password_) : port(port_), password(password_)
{
	create_socket();
}

Serveur::~Serveur()
{

}