#include "../../include/main.hpp"

bool	Server::isValidNickname(std::string nickname)
{
	if (!isalpha(nickname[0]))
		return (false);
	if (nickname.length() > 9)
		return (false);
	if (getUser(nickname) != NULL)
		return (false);
	return (true);
}

bool	Server::isValidUsername(std::string username)
{
	if (!isalpha(username[0]))
		return (false);
	return (true);
}

void	Server::welcome(User * user)
{
	int	fd = user->getFd();

	std::string welcome = user->getNick() + " :Welcome to the Internet Relay Network";
	std::string host = user->getNick() + " :Your host is " + _name + " running version 1.0";
	std::string create = user->getNick() + " This server was created the " + _createTime ;
	std::string info = user->getNick() + " " + _name + " version : 1.0 iwso ov";

	sendForInfo(fd, RPL_WELCOME, welcome);
	sendForInfo(fd, RPL_YOURHOST, host);
	sendForInfo(fd, RPL_CREATED, create);
	sendForInfo(fd, RPL_MYINFO, info);
}

void	Server::addUserInfo(User *user, std::vector<std::string> info)
{
	if (info.size() < 5)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, " USER: Not enough parameters");
		return;
	}
	std::string	realname = info[4].substr(1);

	user->setUsername(info[1]);
	user->setHostname(info[3]);
	for (size_t i = 5; i < info.size(); i++)
	{
		realname += " ";
		realname += info[i];
	}
	user->setRealname(realname);
}
