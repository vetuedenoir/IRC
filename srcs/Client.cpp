#include "Client.hpp"

Client::Client(int sock_fd) : _sock_fd(sock_fd)
{

}

Client::~Client()
{
	close(_sock_fd);
}