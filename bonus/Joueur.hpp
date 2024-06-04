#pragma ONCE

#include <string>
#include "Bot.hpp"

#define	INTRO	0
#define ENCOURS	1
#define	FINI	2

class Joueur
{
	int			_fd;
	int			_status_partie;
	std::string		_nick;
	unsigned int	_nbcoup;
	unsigned int	_secret_number;

 public:
	Joueur (std::string &nick, int fd);
	~Joueur ();

	std::string&	getNick();
	unsigned int&	getNbcoup();
	unsigned int&	getNombre_mystere();
	int&	getStatus_partie();

	void			reset_partie();
	int				the_game(int& guess);



};