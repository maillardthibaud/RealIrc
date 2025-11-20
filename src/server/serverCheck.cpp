#include "../../include/main.hpp"

bool	Server::checkNick(User *user, std::string nickname)
{
	if (nickname.empty())
	{
		sendForInfo(user->getFd(), ERR_NONICKNAMEGIVEN, " :No nickname given");
		return (false);
	}
	if (nickname.size() > 9 || !isalpha(nickname[0]))
	{
		sendForInfo(user->getFd(), ERR_ERRONEUSNICKNAME, (nickname + " :Erroneous nickname"));
		return (false);
	}
	while (getUser(nickname) != NULL)
		nickname += toStr(user->getFd());
	user->setNickname(nickname);
	return (true);
}

bool	Server::checkPassword(User *user, std::vector<std::string> command)
{
	if (command.empty())
		return (false);
	if (command[0] == "PASS")
	{
		if (command.size() < 2)
		{
			sendMessage(user, ERR_NEEDMOREPARAMS, " PASS: Not enough parameters");
			return false;
		}
		if (command[1] == _password)
		{
			user->setCheckPass(true);
			sendMessage(user, NOCODE, "* :Password correct");
			return (true);
		}
		else
		{
			sendForInfo(user->getFd(), ERR_PASSWDMISMATCH, "* :Password incorrect");
			return (false);
		}
	}
	else
		return (false);
}
