/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:18:47 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/09/05 12:00:40 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

Channel::Channel(User * user, std::string name)
	:
	_name(name),
	_nbUsers(1),
	_isInvitOnly(false),
	_topicForOpOnly(true),
	_onPassWordOnly(false),
	_isMaxUsers(false),
	_maxUsers(0),
	_password(""),
	_creator(user->getNick()),
	topic("General"),
	createTime(timestamp())
{
	_users.push_back(user);
	_operators.push_back(user);
}

Channel::~Channel()
{}

void	Channel::delOperator(User* user)
{
	_operators.erase(std::remove(_operators.begin(), _operators.end(), user), _operators.end());
}

bool	Channel::isUserInChannel(User *user)
{
	for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if ((*it) == user)
			return (true);
	}
	return (false);
}

bool	Channel::isOp(User *user)
{
	for (std::vector<User*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		if ((*it) == user)
			return (true);
	}
	return (false);
}

bool	Channel::getIsMaxUsers()
{
	return (_isMaxUsers);
}

void	Channel::setIsMaxUsers(bool value)
{
	_isMaxUsers = value;
}

int		Channel::getMaxUsers()
{
	return (_maxUsers);
}

std::vector<User*>	Channel::getOperators()
{
	return(_operators);
}

void	Channel::setMaxUsers(int nb)
{
	_maxUsers = _maxUsers + nb;
}

bool	Channel::getIsInvit()
{
	return (_isInvitOnly);
}
std::string		Channel::getName()
{
	return (_name);
}

void	Channel::setOperator(User *user)
{
	_operators.push_back(user);
}

int		Channel::getNbUser()
{
	return (_nbUsers);
}

std::vector<User*>          Channel::getUsers()
{
   return (_users);
}

void	Channel::setNbUser(int x)
{
	_nbUsers += x;
}

void            Channel::setUser(User * user)
{
	_users.push_back(user);
}

bool	Channel::isPassCorrect(const std::string &pass)
{
	return (pass == _password);
}

bool	Channel::getOnPassOnly()
{
	return (_onPassWordOnly);
}

bool	Channel::getTopicOpOnly()
{
	return (_topicForOpOnly);
}

void			Channel::setIsInvitOnly(bool value)
{
	_isInvitOnly = value;
}

void			Channel::setTopicForOpOnly(bool value)
{
	_topicForOpOnly = value;
}

void			Channel::setOnPasswordOnly(bool value)
{
	_onPassWordOnly = value;
}

void			Channel::setPassword(std::string password)
{
	_password = password;
}

std::string		Channel::getCreator(void)
{
	return (_creator);
}

void	Channel::setInvite(User *user)
{
	_invites.push_back(user);
}

void Channel::delInvite(User *user)
{
	_invites.erase(std::remove(_invites.begin(), _invites.end(), user), _invites.end());
}

bool Channel::isUserInvited(User *user) const
{
	return std::find(_invites.begin(), _invites.end(), user) != _invites.end();
}
