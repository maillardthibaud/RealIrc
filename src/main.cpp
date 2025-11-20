/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:45:13 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/09/03 04:34:41 by dlacuey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"
#include "../include/signals.hpp"

Server	*ircservPtr = NULL;

static bool isnumber(const std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!isdigit(str[i]))
		{
			return false;
		}
	}
	return true;
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Invalid argument. Usage : ./ircserv <port> <password>\n";
		return (EXIT_FAILURE);
	}
	if (!isnumber(argv[1]))
	{
		std::cerr << "Invalid argument. Usage : ./ircserv <port> <password>\n";
		return (EXIT_FAILURE);
	}
	long port = strtol(argv[1], NULL, 10);
	if (port < 1 || port > 65535)
	{
		std::cerr << "Invalid port number. Port must be between 1 and 65535.\n";
		return (EXIT_FAILURE);
	}
	if (strlen(argv[2]) == 0)
	{
		std::cerr << "Invalid argument. Password cannot be empty.\n";
		return (EXIT_FAILURE);
	}
	setupSignalHandling();
	Server ircserv(argv[1], argv[2]);
	ircservPtr = &ircserv;
	while (ircserv.shutdown_signal == false)
	{
		ircserv.run();
	}
	ircserv.shutdown();
	return (EXIT_SUCCESS);
}
