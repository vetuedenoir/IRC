#include "Bot.hpp"

void send_command(int sockfd, const std::string cmd)
{
    send(sockfd, cmd.c_str(), cmd.size(), 0);
}

message_t	parse_buff(const std::string &buffer)
{

	message_t	msg;
	size_t		limite;
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
		msg.commande =buffer.substr(debut, pos - debut);
	}
	else
		msg.commande = buffer.substr(debut, pos - debut);
	while (buffer[pos] == ' ')
		pos++;
	debut = pos;
	while (debut < limite)
	{
		if (buffer[debut] == ':')
		{
			msg.parametres.push_back(buffer.substr(debut + 1, limite - debut - 1));
			debut = limite;
			break ;
		}
		pos = buffer.find(' ', debut);
		if (limite < pos)
			pos = limite;
		msg.parametres.push_back(buffer.substr(debut, pos - debut));
		while (buffer[pos] == ' ')
			pos++;
		debut = pos;
	}
	return (msg);
}

std::string	extracte_nick(std::string &source)
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

int	 is_digit(char *buffer)
{
	int i = 0;
	while (buffer[i])
	{
		if (!isdigit(buffer[i]))
			return 0;
		i++;
	}
	return (1);
}

	// snprintf(buffer, sizeof(buffer), "PRIVMSG %s :I have chosen a number between 1 and 100. Try to guess it!\r\n", NICK);
	// send_command(sockfd, buffer);
// if (strstr(buffer, "PRIVMSG"))
// {
// 	char *msg_start = strstr(buffer, " :") + 2;
// 	int guess = atoi(msg_start);


void loop_bot(int sockfd)
{
    srand(time(NULL));
    int secret_number = rand() % 100 + 1;
    char buffer[BUFSIZ + 1];
	std::string	nick_of_sender;
	std::map<std::string, Joueur *>	list_joueur;

    while (true)
	{
        memset(buffer, 0, SIZE_BUFF);
        int n = read(sockfd, buffer, SIZE_BUFF);
        if (n <= 0)
		{
            break;
        }
        buffer[n] = '\0';
		message_t msg = parse_buff(buffer);
		if (msg.commande == "ERROR" || msg.commande == "432")
		{
			// 432 c est le rpl de eroneous nickname et Error sa veut dire que le serveur et casser
			std::cout << buffer ;
			return ;
		}
		if (msg.commande == "PRIVMSG")
		{
			nick_of_sender = extracte_nick(msg.source);
			if (nick_of_sender == "")
				continue ;
			if (list_joueur.find(nick_of_sender) != list_joueur.end())
			{
				//le jeu
				//et remove
			}
			else
			{
				Joueur *joueur = new(std::nothrow) Joueur(nick_of_sender, sockfd);
				if (!joueur)
				{
					send();
					continue ;
				}
				list_joueur[nick_of_sender] = joueur;
				

			}

			
		}
		else
			continue ;

		//gerer le control C
		//parsing du buffer split au espace la source, cmd , arg
		//verifie si c est bien un privmsg sinon erreur ou ignore
		//identifier le nick de l envoyeur
		//verifier si joueur existe dans notre liste de joueur si oui on continue la partie sinon on la commence
		//si il a gangne, on le suprime de la liste des joueur ou on lui propose de recommence
		
        std::cout << buffer;

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

std::string init_passw(const std::string &pass)
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
    int port;
    int sockfd;
    std::string pass;
    struct sockaddr_in serv_addr;
    struct hostent *server = NULL;

    if (ac != 3)
    {
        std::cerr << "Error: Utilisation: Bot <port <password>" << std::endl;
        return (1);
    }
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
	send_command(sockfd, "PASS " + pass + " \r\n");
    send_command(sockfd, "NICK " NICK "\r\n");
    send_command(sockfd, "USER " USER "\r\n");

    loop_bot(sockfd);

    close(sockfd);
    return 0;
}
