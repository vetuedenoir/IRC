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

std::vector<std::string>	split_vec(std::string &arg, char c);
std::string uIntToString(size_t value);

std::string longToString(long long value);

#endif