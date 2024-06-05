#pragma once

#include <string>
#include "Bot.hpp"

#define	INTRO	0
#define ENCOURS	1
#define	FINI	2

class Joueur
{
	int				_fd;
	int				_status_partie;
	std::string		_nick;
	unsigned int	_nbcoup;
	int				_secret_number;

 public:
	Joueur (std::string &nick, int fd);
	~Joueur ();

	std::string&	getNick();
	unsigned int&	getNbcoup();
	int&			getNombre_mystere();
	int&			getStatus_partie();
	void			setStatus_partie(int status);
	void			set_secret_num(int secret_number);

	void			reset_partie();
	int				the_game(int guess);
};