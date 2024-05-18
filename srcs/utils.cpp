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


std::vector<std::string>	split_virgule(std::string &arg)
{
	size_t	pos = 0;
	size_t	debut = 0;
	std::string	str;
	std::vector<std::string> list;

	while (pos != std::string::npos)
	{
		debut = pos;
		pos = arg.find(',', debut);
		str = arg.substr(debut, pos - debut);
		list.push_back(str);
		while (arg[pos] == ',')
			pos++;
	}
	return (list);
}
// int	main()
// {
// 	for (long i = 0; i < 100000000; i++)
// 	try
// 	{
// 		std::string str(10, 'a');
// 		casemape(str);
// 		// Casemape(str, str.size());
// 	}
// 	catch(const std::exception& e)
// 	{
// 		exit(1);
// 	}
// 	return 0;

// }