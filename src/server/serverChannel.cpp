#include "../../include/main.hpp"

bool	Server::applyMode(User * user, Channel * channel, std::vector<std::string> command)
{
	std::map<std::string, ModeFunction> modeMap;

	std::map<std::string, ModeFunction>::iterator it = _modeMap.find(command[2]);
	if (it != _modeMap.end())
	{
		ModeFunction func = it->second;
		_isValidMode = true;
		(this->*func)(channel, user, command);
		if (!_isValidMode)
			return (false);
		return (true);
	}
	sendMessage(user, ERR_UNKNOWNCOMMAND, " :Unknown command");
	return (false);
}

void	Server::modeI(Channel * channelId, User *user, std::vector<std::string> command)
{
	(void)user;
	std::string attribut = command[2];
	if (attribut[0] == '+')
		channelId->setIsInvitOnly(true);
	else if (attribut[0] == '-')
		channelId->setIsInvitOnly(false);
}

void	Server::modeT(Channel * channelId, User *user, std::vector<std::string> command)
{
	(void)user;
	std::string attribut = command[2];
	if (attribut[0] == '+')
		channelId->setTopicForOpOnly(true);
	else if(attribut[0] == '-')
		channelId->setTopicForOpOnly(false);
}

void	Server::modeK(Channel * channelId, User *user, std::vector<std::string> command)
{
	std::string attribut = command[2];
	if(!isValidModeK(user, attribut, command))
	{
		_isValidMode = false;
		return ;
	}
	std::string pass = command[3];
	if (attribut[0] == '+')
	{
		channelId->setOnPasswordOnly(true);
		channelId->setPassword(pass);
	}
	else if (attribut[0] == '-' && pass == _password)
		channelId->setOnPasswordOnly(false);
}

void	Server::modeO(Channel * channelId, User *user, std::vector<std::string> command)
{
	if (!isValidModeO(user, command))
	{
		_isValidMode = false;
		return ;
	}
    std::string attribut = command[2];
    std::string nickname = command[3];
    if (attribut[0] == '+')
		channelId->setOperator(getUser(nickname));
    else if (attribut[0] == '-')
		channelId->delOperator(getUser(nickname));
}

void	Server::modeL(Channel * channelId, User *user, std::vector<std::string> command)
{
	std::string attribut = command[2];
	if (isValidModeL(user, attribut, command) == false)
	{
		_isValidMode = false;
		return ;
	}
	if (attribut[0] == '+')
	{
		std::string nbs = command[3];
		char *end;
		long nb = std::strtol(nbs.c_str(), &end, 10);
		channelId->setIsMaxUsers(true);
		channelId->setMaxUsers(nb);
	}
	if (attribut[0] == '-')
		channelId->setIsMaxUsers(false);
}
