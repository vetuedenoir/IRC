#include "utils.hpp"

void	casemape(std::string &str)
{
	size_t	s = str.size();

	for (size_t i = 0; i < s; i++)
	 	str[i] = std::toupper(str[i]);

}

std::string	rcasemape(const std::string &str)
{
	size_t	s = str.size();
	std::string new_str = str;

	for (size_t i = 0; i < s; i++)
	 	new_str[i] = std::toupper(new_str[i]);
	return (new_str);
}

void	run_error(std::string msg)
{
	std::cerr << "Error: ";
	perror(msg.c_str());
}


std::vector<std::string>	split_vec(std::string &arg, char c)
{
	size_t	pos = 0;
	size_t	debut = 0;
	std::string	str;
	std::vector<std::string> list;

	while (pos != std::string::npos)
	{
		debut = pos;
		pos = arg.find(c, debut);
		str = arg.substr(debut, pos - debut);
		list.push_back(str);
		if (pos == std::string::npos)
			break ;
		while (arg[pos] == c)
			pos++;
	}
	return (list);
}

std::string uIntToString(size_t value)
{
    char buffer[11];

    snprintf(buffer, sizeof(buffer), "%zu", value);
    return std::string(buffer);
}

std::string longToString(long long value)
{
    char buffer[20];

    snprintf(buffer, sizeof(buffer), "%lli", value);
    return std::string(buffer);
}
