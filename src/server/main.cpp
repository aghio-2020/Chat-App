#include "serverapplication.h"

#include <iostream>
#include <string>

void helpCliMessage()
{
	std::cout << "- specify ip address and port that the server should listen to\n\n";
	std::cout << "server <ip> <port>\n\n";
	std::cout << "- if you want to make a LAN server enter your device private NAT ip\n";
	std::cout << "- 127.0.0.1 to run a server for local clients\n\n";
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cout << "ERROR - missing args\n\n";
		helpCliMessage();
		return -1;
	}

	uint16_t port = std::strtoul(argv[2], nullptr, 10);
	ServerApplication app(argv[1], port);

	return 0;
}