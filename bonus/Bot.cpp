#include "Bot.hpp"

static int g_signal = 0;

void	sigint_handler(int signo)
{
	(void)signo;
	g_signal = 1;
}

Bot::Bot(std::string pass) : _pass(pass)
{
}

Bot::~Bot()
{
}

void send_command(int sockfd, const std::string cmd)
{
    send(sockfd, cmd.c_str(), cmd.size(), 0);
}

message_t	Bot::parse_buff(const std::string &buffer)
{

	message_t	msg;
	size_t		pos;
	size_t		posr;
	size_t		debut = 0;

	pos = buffer.find(' ', 0);
	posr = buffer.find('\r', 0);

	if (buffer[0] == ':')
	{
		msg.source = buffer.substr(debut + 1, pos - debut - 1);
		while (buffer[pos] == ' ')
			pos++;
		debut = pos;
		pos = buffer.find(' ', debut);
		msg.commande = buffer.substr(debut, pos - debut);
	}
	else
		msg.commande = buffer.substr(debut, pos - debut);
	while (buffer[pos] == ' ')
		pos++;
	debut = pos;
	while (debut < posr)
	{
		if (buffer[debut] == ':')
		{
			msg.parametres.push_back(buffer.substr(debut + 1, posr - debut - 1));
			debut = posr;
			break ;
		}
		pos = buffer.find(' ', debut);
		if (posr < pos)
			pos = posr;
		msg.parametres.push_back(buffer.substr(debut, pos - debut));
		while (buffer[pos] == ' ')
			pos++;
		debut = pos;
	}
	return (msg);
}

std::string	Bot::extracte_nick(std::string &source)
{
	size_t	pos;
	std::string	nick;

	if (source.empty())
		return ("");
	pos = source.find('!', 0);
	if (pos == std::string::npos)
		return ("");
	nick = source.substr(0, pos);
	return (nick);
}

int	 Bot::is_digit(const char *buffer)
{
	int i = 0;
	while (buffer[i])
	{
		if (!isdigit(buffer[i]))
			return (0);
		i++;
	}
	return (1);
}

void	Bot::print_message(message_t &msg)
{
	if (msg.commande == "")
		return ;
	std::cout << "-------message---------------------------------" << std::endl;
	std::cout << msg.source << "|" << std::endl;
	std::cout << msg.commande << "|" <<  std::endl;
	for (size_t i = 0; i < msg.parametres.size(); i++)
		std::cout << "x " << msg.parametres[i] << "|" << std::endl;
}

void Bot::process_command(message_t msg, int sockfd)
{
	std::string	nick_of_sender;
    int 		secret_number;

	nick_of_sender = extracte_nick(msg.source);
	if (nick_of_sender == "")
		return ;
	std::map<std::string, Joueur >::iterator it = list_joueur.find(nick_of_sender);
	if (it != list_joueur.end())
	{
		if (msg.parametres.size() < 1)
			return ;
		if (it->second.getStatus_partie() == ENCOURS)
		{
			if (is_digit(msg.parametres[1].c_str()) == 0)
				send_command(sockfd, "PRIVMSG " + nick_of_sender + " : Please give me a number between 0 and 500\r\n");
			else if (it->second.the_game(atoi(msg.parametres[1].c_str())) == FINI)
				list_joueur.erase(it);
		}
	}
	else
	{
		if (msg.parametres.size() > 1 && (msg.parametres[1].find("game") != std::string::npos || msg.parametres[1].find("jeu")!= std::string::npos ||
			(msg.parametres[1].find("GAME") != std::string::npos  || msg.parametres[1].find("JEU") != std::string::npos )))
		{
			list_joueur.insert(std::make_pair(nick_of_sender, Joueur(nick_of_sender, sockfd)));
			std::map<std::string, Joueur>::iterator it = list_joueur.find(nick_of_sender);
			if (it != list_joueur.end())
			{
				it->second.setStatus_partie(ENCOURS);
				secret_number = rand() % 500 + 1;
				it->second.set_secret_num(secret_number);
			}
			send_command(sockfd, "PRIVMSG " + nick_of_sender + " : Guess a number between 0 and 500\r\n");
		}
		else
			send_command(sockfd, "PRIVMSG " + nick_of_sender + " : Send game or jeu if you want to play\r\n");
	}
}

void Bot::loop_bot(int sockfd)
{
	srand(time(NULL));
	char buffer[BUFSIZ + 1];
	
	while (!g_signal)
	{
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(sockfd, &read_fds);

		// Définir un délai d'attente pour la fonction select
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		// Utiliser select pour attendre sur la socket et le signal de contrôle C
		int ret = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
		if (ret == -1)
		{
			perror("select");
			break ;
		}
		else if (ret > 0)
		{
			// Des données sont disponibles sur la socket
			if (FD_ISSET(sockfd, &read_fds))
			{
				memset(buffer, 0, SIZE_BUFF);
				int n = recv(sockfd, buffer, SIZE_BUFF, 0);
				if (n <= 0)
					break ;
				buffer[n] = '\0';
				message_t msg = parse_buff(buffer);
				if (msg.commande == "ERROR" || msg.commande == "433")
				{
					std::cout << buffer ;
					break ;
				}
				if (msg.commande == "PRIVMSG")
					process_command(msg, sockfd);
			}
		}	
	}
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
	if (port < 1024)
		throw	"Erreur: invalide port";
	return (port);
}

std::string	init_passw(const std::string &pass)
{
	if (pass.size() < 1)
	 	throw "Erreur MDP: trop court, minimum 6 caracteres";
	if (pass.size() > 505)
		throw "Erreur MDP: trop long, maximum 505 caracteres";
	char	c;
	for (size_t i = 0; i < pass.size(); i++)
	{
		c = pass[i];
		if (!isgraph(c))
			throw "Erreur MDP: uniquement des caracteres graphique accepter";
	}
    return (pass);
}

int main(int ac, char **av)
{
	int					port;
	int					sockfd;
	std::string			pass;
	struct sockaddr_in	serv_addr;
	struct hostent		*server = NULL;

	if (ac != 3)
	{
		std::cerr << "Error: Utilisation: Bot <port <password>" << std::endl;
		return (1);
	}
	std::signal(SIGINT, &sigint_handler);
	std::signal(SIGQUIT, &sigint_handler);
	try
	{
		pass = init_passw(av[2]);
		port = init_port(av[1]);
	}
	catch(const char * e) {
		std::cerr << e << '\n';
		return (1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::cerr << "ERROR opening socket" << std::endl;
		return 1;
	}
    server = gethostbyname(SERVER);
	if (server == NULL)
	{
		std::cerr << "ERROR, no such host" << std::endl;
		return 1;
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	serv_addr.sin_port = htons(port);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "ERROR connecting" << std::endl;
		return 1;
	}
	
	Bot bot(pass);
	send_command(sockfd, "PASS " + pass + " \r\n");
	send_command(sockfd, "NICK " NICK "\r\n");
	send_command(sockfd, "USER " USER "\r\n");
	try
	{
		bot.loop_bot(sockfd);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	close(sockfd);
	return (0);
}
