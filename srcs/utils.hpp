#ifndef UTILS_HPP
#define UTILS_HPP

#include <cctype>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>



void	casemape(std::string &str);
std::string	rcasemape(const std::string &str);
void	run_error(std::string msg);

std::vector<std::string>	split_virgule(std::string &arg);
#endif