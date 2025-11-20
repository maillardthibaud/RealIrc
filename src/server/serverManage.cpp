#include "../../include/main.hpp"

void	Server::manageInput(User * user, std::vector<std::string> command)
{
	// std::vector<std::string> command = parsingIntoVector(input);
	std::map<std::string, ServerFunction>::iterator it = _commandMap.find(command[0]);
	if (it != _commandMap.end()) 
	{
		ServerFunction func = it->second;
		(this->*func)(user, command);
	}
	else 
	{
		sendMessage(user, ERR_UNKNOWNCOMMAND, ":Unknown command");
	}
}

bool	Server::manageUser(User *user, std::vector<std::string> command) 
{
	if (command.empty() || command.size() < 2)
		return false;
	if (command[0] == "NICK")
	{
		if (checkNick(user, command[1]) == false)
			return (false);
	}
	if (command[0] == "USER")
	{
		addUserInfo(user, command);
		return (true);
	}
	else
		return (false);
}

void	Server::manageBuff(std::string input, User *user)
{
	int	fd = user->getFd();
	input = user->getPartialMessage() + input;
	user->clearPartialMessage();
	size_t pos = 0;
	int i = 0;
	while (i < _nfds)
	{
		if (fd == _fds[i].fd)
			break;
		i++;
	}
	while ((pos = input.find("\r\n")) != std::string::npos)
	{
		std::string completeMsg = input.substr(0, pos);
		input.erase(0, pos + 2);
		std::vector<std::string> command = parsingIntoVector(completeMsg);
		if (user->checkUserInfo() == false)
		{
			if (user->getCheckPass() == false)
				checkPassword(user, command);
			manageUser(user, command);
			if (user->checkUserInfo())
				welcome(user);
		}
		else
		{
			// _fds[i].revents = POLLOUT;
			manageInput(user, command);
			std::cout << "Received message from client " << fd << ": " << completeMsg << std::endl;
			// _fds[i].revents = POLLIN;
		}
	}

	if (!input.empty())
		user->setPartialMessage(input);
}
