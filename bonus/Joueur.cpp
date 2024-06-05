#include "Joueur.hpp"

Joueur::Joueur(std::string &nick, int fd) : _fd(fd), _nick(nick) 
{
	_status_partie = 0;
	_nbcoup = 0;
	//envoyer msg d intro et demande si il veut jouer
}

Joueur::~Joueur () 
{
}


std::string&	Joueur::getNick()
{
	return (_nick);
}

unsigned int&	Joueur::getNbcoup()
{
	return (_nbcoup);
}

int&	Joueur::getNombre_mystere()
{
	return (_secret_number);
}

int&	Joueur::getStatus_partie()
{
	return (_status_partie);
}

void	Joueur::setStatus_partie(int status)
{
	_status_partie = status;
}

void	Joueur::set_secret_num(int secret_number)
{
	_secret_number = secret_number;
}

void	Joueur::reset_partie()
{

}

int	Joueur::the_game(int guess)
{
	char buffer[SIZE_BUFF + 1];

	if (guess == _secret_number)
	{
		snprintf(buffer, SIZE_BUFF, "PRIVMSG %s :Congratulations! You guessed the number: %d in %d attempt(s) \r\n", _nick.c_str(), _secret_number, _nbcoup);
		send_command(_fd, buffer);
		_status_partie = FINI;
		return (FINI);
	}
	else if (guess < _secret_number)
	{
		snprintf(buffer, SIZE_BUFF, "PRIVMSG %s :Higher!\r\n", _nick.c_str());
		send_command(_fd, buffer);
		_nbcoup++;
		return (ENCOURS);
	}
	else
	{
		snprintf(buffer, SIZE_BUFF, "PRIVMSG %s :Lower!\r\n", _nick.c_str());
		send_command(_fd, buffer);
		_nbcoup++;
		return (ENCOURS);
	}

}