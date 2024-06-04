#include "Joueur.hpp"

Joueur::Joueur(std::string &nick, int fd) : _nick(nick), _fd(fd)
{
	_status_partie = 0;
	//envoyer msg d intro et demande si il veut jouer
}

Joueur::~Joueur () {}


std::string&	Joueur::getNick()
{
	return (_nick);
}

unsigned int&	Joueur::getNbcoup()
{
	return (_nbcoup);
}

unsigned int&	Joueur::getNombre_mystere()
{
	return (_secret_number);
}

int&	Joueur::getStatus_partie()
{
	return (_status_partie);
}

void	Joueur::reset_partie()
{

}

int	Joueur::the_game(int &guess)
{
	char *buffer;

	if (guess == _secret_number)
	{
		snprintf(buffer, SIZE_BUFF, "PRIVMSG %s :Congratulations! You guessed the number: %d\r\n", _nick, _secret_number);
		send_command(_fd, buffer);
		_status_partie = FINI;
		return (FINI);
	}
	else if (guess < _secret_number)
	{
		snprintf(buffer, SIZE_BUFF, "PRIVMSG %s :Higher!\r\n", _nick);
		send_command(_fd, buffer);
		return (ENCOURS);
	}
	else
	{
		snprintf(buffer, SIZE_BUFF, "PRIVMSG %s :Lower!\r\n", _nick);
		send_command(_fd, buffer);
		return (ENCOURS);
	}

}