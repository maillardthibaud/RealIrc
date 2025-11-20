/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:03:04 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/09/03 13:06:47 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

User::User()
	:
	_realname(""),
	_username(""),
	_nickname(""),
	_hostname(""),
	_fd(-1),
	_checkPass(false),
	deleteMe(false)
{}

User::~User()
{
	std::cout << this->_nickname << " has quit the server" << std::endl;
}

std::string		User::getUsername(void)
{
	return (_username);
}

std::string		User::getNick(void)
{
	return (_nickname);
}

std::string		User::getHostname()
{
	return (_hostname);
}

std::string		User::getReal()
{
	return (_realname);
}

void	User::setNickname(std::string nickname)
{
	_nickname = nickname;
}

void	User::setFd(int fd)
{
	_fd = fd;
}

int	User::getFd()
{
	return (_fd);
}

void	User::setPartialMessage(std::string str)
{
	_partialMessage = str;
}

std::string	User::getPartialMessage()
{
	return (_partialMessage);
}

void	User::clearPartialMessage()
{
	_partialMessage.clear();
}

bool	User::getCheckPass()
{
	return (_checkPass);
}

void	User::setCheckPass(bool pass)
{
	_checkPass = pass;
}

void	User::setUsername(std::string username)
{
	_username = username;
}

void	User::setRealname(std::string realname)
{
	_realname = realname;
}

void	User::setHostname(std::string hostname)
{
	_hostname = hostname;
}

bool	User::checkUserInfo()
{
	if (_nickname.empty())
		return (false);
	if (_realname.empty())
		return (false);
	if (_username.empty())
		return (false);
	if (_hostname.empty())
		return (false);
	if (_checkPass == false)
		return (false);
	return (true);
}