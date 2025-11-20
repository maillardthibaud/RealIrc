/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:38:47 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/09/11 09:14:13 by dlacuey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/main.hpp"

Server::Server(std::string port, std::string password)
	:
	_nfds(0),
	_port(port),
	_password(password),
	_createTime(""),
	_nbClient(0),
	_nbChannel(0),
	_name("myIRC"),
	_isValidMode(true),
	shutdown_signal(false)
{
	_createTime = timestamp();
	for (int i = 0; i < MAXCLIENT; ++i) {
		_fds[i].fd = -1;
		_fds[i].events = 0;
		_fds[i].revents = 0;
	}
	initializeCommandMap();
	initializeModeMap();
}

Server::~Server()
{
	std::cout << "Server destructor" << std::endl;
	if (_connection.socketFd != -1)
	{
		close(_connection.socketFd);
		_connection.socketFd = -1;
	}
	for (int i = 0; i < _nfds; i++)
	{
		if (_fds[i].fd != -1)
		{
			close(_fds[i].fd);
			_fds[i].fd = -1;
		}
	}
}

void Server::initializeCommandMap()
{
	_commandMap["NICK"] = &Server::nick;
	_commandMap["userhost"] = &Server::userName;
	_commandMap["USER"] = &Server::userName;
	_commandMap["JOIN"] = &Server::join;
	_commandMap["PRIVMSG"] = &Server::msg;
	_commandMap["KICK"] = &Server::kick;
	_commandMap["INVITE"] = &Server::invite;
	_commandMap["TOPIC"] = &Server::defTopic;
	_commandMap["MODE"] = &Server::mode;
	_commandMap["PING"] = &Server::ping;
	_commandMap["PART"] = &Server::leave;
	_commandMap["WHO"] = &Server::who;
	_commandMap["WHOIS"] = &Server::whois;
	_commandMap["LIST"] = &Server::list;
	_commandMap["QUIT"] = &Server::quit;
	_commandMap["info"] = &Server::info;
}

void	Server::initializeModeMap()
{
	_modeMap["+i"] = &Server::modeI;
	_modeMap["-i"] = &Server::modeI;
	_modeMap["+k"] = &Server::modeK;
	_modeMap["-k"] = &Server::modeK;
	_modeMap["+t"] = &Server::modeT;
	_modeMap["-t"] = &Server::modeT;
	_modeMap["+o"] = &Server::modeO;
	_modeMap["-o"] = &Server::modeO;
	_modeMap["+l"] = &Server::modeL;
	_modeMap["-l"] = &Server::modeL;
}

