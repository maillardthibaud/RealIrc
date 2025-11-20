#include "../../include/main.hpp"

void	Server::nick(User *user, std::vector<std::string> command)
{
	if (command.size() != 2)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, " NICK: Not enough parameters");
		return ;
	}
	if (!isValidNickname(command[1]))
	{
		sendMessage(user, ERR_ERRONEUSNICKNAME, ("Erroneous " + command[1]));
		return ;
	}
	std::string oldNick = user->getNick();
	std::string newNick = command[1];
	std::string msg = "Your nickname was " + oldNick + " its now  " + newNick;
	user->setNickname(newNick);
	sendMessage(user, RPL_MYINFO, msg);
	std::cout << oldNick << " has changed his nickname to : " << newNick << std::endl;
}

void	Server::userName(User *user, std::vector<std::string> command)
{
	if (command.size() != 5)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
		return ;
	}
	if (!isValidUsername(command[1]))
	{
		sendMessage(user, ERR_ERRONEUSNICKNAME, ("Erroneous " + command[1]));
		return ;
	}
	std::string newName = command[1];
	std::string oldName = user->getUsername();
	user->setUsername(newName);
	user->setHostname(command[2]);
	user->setRealname(command[4]);
	std::string msg = "You username was : " + oldName + ", it's now : " + newName;
	sendMessage(user, RPL_MYINFO, msg);
	std::cout << oldName << " has changed his username to : " << newName << std::endl;
}

bool	Server::isValidChannelName(std::string name)
{
	if (name.length() > 200)
		return (false);
	if (name[0] == '#' || name[0] == '&')
	{
		for (size_t i = 0; i < name.size(); i++)
			if(!isprint(name[i]))
				return (false);
		return (true);
	}
	return (false);
}

void	Server::join(User *user, std::vector<std::string> command)
{
	if (command.size() < 2)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, " JOIN: Not enough parameters");
		return ;
	}
	std::string name = command[1];
	if (!isValidChannelName(name))
	{
		sendMessage(user, ERR_NOSUCHCHANNEL, (name + " :Not such Channel"));
		return ;
	}
	if (isValidChannel(name) == false)
		createNewChannel(user, name);
	else
		joinChannel(user, command, name);

}

void	Server::deleteChannel(Channel *channel)
{
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end();)
	{
		if ((*it) == channel)
		{
			it = _channels.erase(it);
			delete(channel);
			--_nbChannel;
		}
		else
			++it;
	}
}

void	Server::leave(User *user, std::vector<std::string> command)
{
	if (command.size() < 2)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, " PART: Not enough parameters");
		return;
	}
	std::string channelName = command[1];
	Channel *channel = getChannel(channelName);

	if (channel == NULL)
		return;
	if (!channel->isUserInChannel(user))
		return;
	channel->removeUser(user);
	channel->setNbUser(-1);
	channel->delInvite(user);
	channel->delOperator(user);
	std::string msg = ("PART " + channelName + " :Leaving the channel");
	if (command[0] != "KICK" && command[0] != "BAN")
	{
		sendMsgToChanFromUser(channel, user, msg);
		SendSpeMsg(user, user, msg);
	}
	if (channel->getNbUser() == 0)
		deleteChannel(channel);
}

void Channel::removeUser(const std::string& nick)
{
	for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if ((*it)->getNick() == nick)
			it = _users.erase(it);
	}
}

void	Channel::removeUser(User* user)
{
	for (std::vector<User*>::iterator it = _users.begin(); it != _users.end();)
	{
		if (*it == user)
			it = _users.erase(it);
		else
			++it;
	}
}

void	Server::who(User *user, std::vector<std::string> command)
{
	if (command.size() > 2)
	{
		sendMessage(user, ERR_UNKNOWNCOMMAND, "WHO :Unknow command");
		return ;
	}
	std::string channel = command[1];
	if (isValidChannel(channel) == false)
	{
		sendMessage(user, ERR_NOSUCHCHANNEL, (channel + ":No such channel"));
		return ;
	}
	Channel *curr = getChannel(channel);
	std::vector<User*> listUsers = curr->getUsers();
	for (std::vector<User*>::iterator it = listUsers.begin(); it != listUsers.end(); it++)
	{
		std::string nick = (*it)->getNick();
		std::string username = (*it)->getUsername();
		std::string host = (*it)->getHostname();
		std::string real = (*it)->getReal();
		std::string flag;
		if (curr->isOp((*it)) == true)
			flag = "H@";
		else
			flag = "H";
		std::string	msg = channel + " ~" + username + " " + host + " " + _name + " " + nick + " " + flag + " :0"
		+ " " + real;  
		sendMessage(user, RPL_WHOREPLY, msg);
	}
	sendMessage(user, RPL_ENDOFWHO, (channel + ":End of /WHO list"));
}

int		Server::privOrChan(std::string input)
{
	if (input[0] == '&' || input[0] == '#')
		return (2);
	else
		return (1);
}

void	Server::privateMsg(User *user, std::vector<std::string> command)
{
	//A REVOIR, PAS FONCTIONNEL ATM
	std::string message;
	User*		userToSend = getUser(command[1]);

	if (userToSend == NULL)
	{
		sendMessage(user, ERR_NOSUCKNICK, "No such nick");
		return;
	}
	for(size_t i = 2; i < command.size(); i++)
	{
		message += command[i];
		message += " ";
	}
	trimTrailingSpace(message);
	std::string msg = "PRIVMSG " + userToSend->getNick() + " " + message; 
	SendSpeMsg(user, userToSend, msg);
}

void    Server::channelMsg(User *user, std::vector<std::string> command)
{
	//IL NE FAUT PAS VECTORISER LA COMMANDE DE LA MANIERE ACTUELLE CAR
	//CELA VIRE LES ESPACES
	//lol         lol -> lol lol

	Channel		*channel = getChannel(command[1]);
	std::string name = command[1];
	std::string message;

	for(size_t i = 2; i < command.size(); i++)
	{
		message += command[i];
		message += " ";
	}
	trimTrailingSpace(message);
	if (message.empty())
	{
		sendMessage(user, ERR_NOTEXTTOSEND, ":No text to send");
	}
	if (channel == NULL)
	{
		sendMessage(user, ERR_NOSUCHCHANNEL, (name + " :No such channel"));
	}
	else if (channel->isUserInChannel(user) == false)
	{
		sendMessage(user, ERR_CANNOTSENDTOCHAN, (name + " :Cannot send to channel"));
	}
	else if (channel->getNbUser() > 1)
	{
		sendMsgToChanFromUser(channel, user, "PRIVMSG " + name + " " + message);
	}
}

void	Server::msg(User *user, std::vector<std::string> command)
{
	if (command.size() < 3)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, " PRIVMSG: Not enough parameters");
		return;
	}
	int choice = privOrChan(command[1]);
	//PRIVATE MSG PAS FONCTIONNEL ATM
	if (choice == 1)
		privateMsg(user, command);
	else
		channelMsg(user, command);
}

void	Server::list(User *user, std::vector<std::string> /*command*/)
{
	std::string msg;
	if (_nbChannel == 0)
		sendMessage(user, RPL_LISTEND, " :End of LIST");
	else
	{      
		for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		{
			std::string name = (*it)->getName();
			std::string topic = (*it)->topic;
			std::string nb = toStr((*it)->getNbUser());
			msg = name + " " + nb + " :" + topic;
			sendMessage(user, RPL_LIST, msg);
		}
		sendMessage(user, RPL_LISTEND, " :End of LIST");
	}
}

std::string	Server::getUserChannels(User *user)
{
	std::string str = "";
	
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it)->isOp(user))
			str += "@";
		if ((*it)->isUserInChannel(user))
			str += (*it)->getName() + " ";
	} 
	return (str);
}

void	Server::kick(User *user, std::vector<std::string> command)
{
	if (command.size() < 3)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, " KICK: Not enough parameters");
		return;
	}
	if (!checkKickInfo(user, command))
		return ;

	std::string channelName = command[1];
	Channel *channel = getChannel(channelName);
	User * toKick = getUser(command[2]);

	std::string msg = "KICK " + channelName + " " + command[2];
	sendSpeMsgToAllUsersInChannel(channel, user, msg);
	channel->delOperator(toKick);
	leave(toKick, command);
}

bool	Server::checkInviteCommand(User *user, std::string nickTo, std::string channelStr)
{
	Channel *channel = getChannel(channelStr);
	User	*userTo = getUser(nickTo);

	if (channel == NULL)
	{
		sendMessage(user, ERR_NOSUCHCHANNEL, (channelStr + " :No such channel"));
		return (false);
	}
	if (channel->isOp(user) == false)
	{
		sendMessage(user, ERR_CHANOPRIVSNEEDED, (channelStr + " :You're not channel operator"));
		return (false);
	}
	if(userTo == NULL)
	{
		sendMessage(user, ERR_NOSUCKNICK, (nickTo + " : No such nick"));
		return (false);
	}
	if (channel->isUserInChannel(userTo) == true)
	{
		sendMessage(user, ERR_USERONCHANNEL, (nickTo + " " + channelStr + " :is already on channel"));
		return (false);
	}
	return (true);
}

void	Server::invite(User *host, std::vector<std::string> command)
{
	if (command.size() < 3)
	{
		sendMessage(host, ERR_NEEDMOREPARAMS, " INVITE: Not enough parameters");
		return; 
	}
	std::string inviteeNick = command[1];
	std::string hostChannel = command[2];

	if (!checkInviteCommand(host, inviteeNick, hostChannel))
		return ;
	Channel *channel = getChannel(hostChannel);
	User * invitee = getUser(inviteeNick);
	std::string msg = "INVITE " + inviteeNick + " :" + hostChannel;
	sendMessage(host, RPL_INVITING, (inviteeNick + " " + hostChannel));
	SendSpeMsg(host, invitee, msg);
	channel->setInvite(invitee);
		//AJOUTER LA LISTE DES INVITES DANS CHANNEL PLUTOT QUE DANS USER
		//COMME CA QUAND TU CHECK DANS JOIN, TU CHECK DANS LE CHANNEL
		//SI USER EST AUTORISE A ENTRER
		// toSend->getListInvit().insert(std::make_pair(channelTo, true));
}

void    Server::defTopic(User *user, std::vector<std::string> command)
{
	if (command.size() < 3)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, " TOPIC: Not enough parameters");
		return;
	}
	std::string channelName = command[1];
	Channel * channel = getChannel(channelName);

	if (!checkTopicCommand(user, channel, channelName))
		return ;
	else if (command.size() >= 3)
	{
		std::string newTopic = mergeCommand(command);
        channel->topic = newTopic;
		channel->createTime = timestamp();
		std::string nick = user->getNick();
		std::vector<User*> users = channel->getUsers();
		for (std::vector<User*>::iterator it = users.begin(); it != users.end(); it++)
		{
			sendMessage((*it), RPL_TOPIC, (channelName + " :" + newTopic));
			sendMessage((*it), RPL_TOPICWHOTIME, (channelName + " " + nick + " " + channel->createTime));
		}
	}
}

void	Server::ping(User *user, std::vector<std::string> /*command*/)
{
	sendMessage(user, NOCODE, ("PONG " + _name));
}

bool	Server::checkModeCommand(User* user, std::vector<std::string> command)
{
	if (command.size() < 2)
		return false;
	Channel *channel = getChannel(command[1]);

	if (command[1] == user->getNick())
		return (false);
	if (command.size() < 2 || command.size() > 4)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, " MODE: Not enought parameters");
		return (false);
	}
	if (channel == NULL)
	{
		sendMessage(user, ERR_NOSUCHCHANNEL, (command[1] + " :No such channel"));
		return (false);
	}
	if (channel->isOp(user) == false)
	{
		sendMessage(user, ERR_CHANOPRIVSNEEDED, " :You're not channel operator");
		return (false);
	}
	return (true);
}

void	Server::mode(User *user, std::vector<std::string> command)
{
	if (command.size() < 2)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, " MODE: Not enough parameters");
		return;
	}
	if (command.size() == 2)
	{
		if (!isValidChannel(command[1]))
			return ;
		Channel * curr = getChannel(command[1]);
		if (curr->isOp(user))
			sendMessage(user, RPL_CHANNELMODEIS, (command[1] + " +nto " + user->getNick()));
		else
			sendMessage(user, RPL_CHANNELMODEIS, command[1] + " +nt");
		return;
	}
	if (!checkModeCommand(user, command))
		return ;
	std::string channelName = command[1];
	Channel * channel = getChannel(channelName);
	std::string attribut = command[2];
	if (applyMode(user, channel, command) == true)
	{
		std::string msg = "MODE " + channelName + " " + attribut;
		if (command.size() > 3)
			msg += " :" + command[3];
		sendSpeMsgToAllUsersInChannel(channel, user, msg);
	}
}
bool	Server::isValidModeK(User *user, std::string attribut, std::vector<std::string> command)
{
	if (attribut == "+k" || attribut == "-k")
	{
		if (command.size() >= 4)
		{
			if (command[3].size() > 16)
			{
				sendMessage(user, NOCODE, " :Too long pasword, max 16 caracteres");
				return (false);
			}
			return (true);
		}
		else
		{
			sendMessage(user, ERR_NEEDMOREPARAMS, " :Not enough parameters");
			return (false);
		}
	}
	return (true);
}

bool    Server::isValidModeO(User *user, std::vector<std::string> command)
{
	if (command.size() < 4)
	{
		sendMessage(user, ERR_NEEDMOREPARAMS, "MODE: Need more parameters");
		return (false);
	}
	std::string	channelName = command[1];
	User		*userTo = getUser(command[3]);
	Channel		*channel = getChannel(channelName);
	if (userTo == NULL)
	{
		sendMessage(user, ERR_NOSUCKNICK, " :No such nickname");
		return (false);
	}

	if (channel->isUserInChannel(userTo))
		return (true);
	sendMessage(user, ERR_USERNOTINCHANNEL, " :They aren't on that channel");
	return (false);
}

bool	Server::isValidModeL(User *user, std::string attribut, std::vector<std::string> command)
{
	if (attribut == "+l")
	{
		if (command.size() >= 4)
		{
			std::string infoNb = command[3];
			char *end;
			long nb = std::strtol(infoNb.c_str(), &end, 10);
			if (nb <= 0 || nb > MAXCLIENT)
			{
				sendMessage(user, ERR_UNKNOWNMODE, " :Invalid command");
				return (false);
			}
			return (true);
		}
		else
		{
			sendMessage(user, ERR_NEEDMOREPARAMS, " :Not enough parameters");
			return (false);
		}
	}
	if (attribut == "-l")
	{
		if (command.size() == 3)
			return (true);
		else
		{
			sendMessage(user, ERR_NEEDMOREPARAMS, " :Not enough parameters");
			return (false);       
		}
	}
	return (true);
}

// bool	Server::isValidArgv(User *user, std::string attribut, std::vector<std::string> command)
// {
// 	if (isValidModeK(user, attribut, command) == false)
// 		return (false);
// 	// if (isValidModeO(user, attribut, command) == false)
// 		// return (false);
// 	if (isValidModeL(user, attribut, command) == false)
// 		return (false);
// 	return (true);
// }


void	Server::whois(User *user, std::vector<std::string> command)
{
	std::string nickname;

	if (command.size() == 1)
		nickname = user->getNick();
	nickname = command[1];

	User		*userTo = getUser(nickname);

	sendMessage(user, RPL_WHOISUSER, nickname + " " + userTo->getUsername() + " " + userTo->getHostname() + " * :" + userTo->getReal());
	sendMessage(user, RPL_WHOISCHANNELS, nickname + " :" + getUserChannels(userTo));
	sendMessage(user, RPL_ENDOFWHOIS, (" :End of /WHOIS list"));
}

std::string		Server::mergeCommand(std::vector<std::string> command)
{
	std::string result;
	size_t i = 2;
	while(i <= command.size() - 2)
	{
		result += command[i] + " ";
		i++;
	}
	result += command[i];
	result.erase(0, 1);
	return (result);
}

bool	Server::checkTopicCommand(User *user, Channel * channelId, std::string channel)
{
	if (channelId == NULL)
	{
		sendMessage(user, ERR_NOSUCHCHANNEL, (channel + " :No such channel"));
		return (false);       
	}
	if (channelId->isOp(user) == false && channelId->getTopicOpOnly() == true)
	{
		sendMessage(user, ERR_CHANOPRIVSNEEDED, " :You're not channel operator");
		return (false);
	}
	return (true);
}

void	Server::createNewChannel(User *user, std::string name)
{
    Channel *newChannel = new Channel(user, name);
	++_nbChannel;
	SendSpeMsg(user, user, ("JOIN :" + name));
    sendMessage(user, RPL_TOPIC, (name + " " + newChannel->topic));
    std::string time = newChannel->createTime;
    sendMessage(user, RPL_TOPICWHOTIME, (name + " " + user->getNick() + " " + time));
	sendMessage(user, RPL_CREATIONTIME, (name + " " + time));
    sendMessage(user, RPL_NAMREPLY, ("= " + name + " :@" + user->getNick()));
    sendMessage(user, RPL_ENDOFNAMES, (name + " :End of /NAMES list"));
	_channels.push_back(newChannel);
}

bool	Server::checkInvit(User *user, std::string channelName)
{
	Channel *channel = getChannel(channelName);
	if (channel)
		return (channel->isUserInvited(user));
	return (false);
}

bool	Server::checkJoinChannel(User *user, Channel * channelId, std::vector<std::string> command)
{
	if (channelId->getIsInvit() == true)
	{
		if (checkInvit(user, command[1]) == false)
		{
			sendMessage(user, ERR_INVITEONLYCHAN, (channelId->getName() + " :Cannot join channel (+i)"));
			return (false);
		}
	}
	if (channelId->getOnPassOnly() == true)
	{
		if (command.size() != 3)
		{
			sendMessage(user, ERR_BADCHANNELKEY, (channelId->getName() + " :Cannot join channel (+k)"));
			return (false);
		}
		if (command.size() >= 3)
		{
			std::string pass = command[2];
			if (channelId->isPassCorrect(pass) == false)
			{
				sendMessage(user, ERR_BADCHANNELKEY, (channelId->getName() + " :Cannot join channel (+k)"));
				return (false);           
			}
		}
	}
	if (channelId->getIsMaxUsers() == true)
	{
		if (channelId->getNbUser() + 1 > channelId->getMaxUsers())
		{
			sendMessage(user, ERR_CHANNELISFULL, (_name + " :Cannot join channel (+l)"));
			return (false);
		}
	}
	return (true);
}

void	Server::joinChannel(User *user, std::vector<std::string> command, std::string name)
{
	Channel * channel = getChannel(name);
	if (checkJoinChannel(user, channel, command) == false)
		return ;
	channel->setNbUser(1);
	channel->setUser(user);
	SendSpeMsg(user, user, ("JOIN :" + name));
    sendMessage(user, RPL_TOPIC, (name + " " + channel->topic));
    sendMessage(user, RPL_TOPICWHOTIME, (name + " " + channel->getCreator() + " " + channel->createTime));
	sendMessage(user, RPL_CREATIONTIME, (name + " " + channel->createTime));
	std::string listUser = getUsersInChannelStr(channel);
    sendMessage(user, RPL_NAMREPLY, ("= " + name + " :" + listUser));
    sendMessage(user, RPL_ENDOFNAMES, (name + " :End of /NAMES list"));
	if (channel->isUserInvited(user))
		channel->delInvite(user);
	sendSpeMsgToAllUsersInChannelExceptOne(channel, user, "JOIN :" + name);
}

std::string		Server::getUsersInChannelStr(Channel * curr)
{
	std::vector<User*>			usersInChannelPtr = curr->getUsers();
	std::vector<std::string>	usersInChannelStr;
	std::string					msg;

	for (std::vector<User*>::iterator it = usersInChannelPtr.begin(); it != usersInChannelPtr.end(); it++)
	{
		if (curr->isOp(*it))
			usersInChannelStr.push_back("@" + (*it)->getNick());
		else
			usersInChannelStr.push_back((*it)->getNick());
	}
	// std::sort(usersInChannelStr.begin(), usersInChannelStr.end());

	for(std::vector<std::string>::iterator it = usersInChannelStr.begin(); it != usersInChannelStr.end(); it++)
	{
		msg += (*it + " ");
	}
	msg.erase(msg.size() - 1);
	return (msg);
}

bool	Server::isValidChannel(std::string name)
{
	if (getChannel(name) != NULL)
		return (true);
	return (false);
}

Channel*	Server::getChannel(std::string name)
{
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->getName() == name)
			return (*it);
	}
	return (NULL);
}

bool	Server::checkKickInfo(User *user, std::vector<std::string> command)
{
	Channel	*channel = getChannel(command[1]);
	User	*toKick = getUser(command[2]);

	if (command.size() != 4)
	{
		sendMessage(user, ERR_UNKNOWNCOMMAND, " :Unknow command");
		return (false);
	}
	if (channel == NULL)
	{
		sendMessage(user, ERR_NOSUCHCHANNEL, (command[1] + " :No such channel"));
		return (false);
	}
	if (user == NULL)
	{
		sendMessage(user, ERR_NOSUCKNICK, (command[1] + " :No such nick"));
		return (false);
	}
	if (channel->isOp(user) == false)
	{
		sendMessage(user, ERR_CHANOPRIVSNEEDED, (command[2] + " You're not channel operator"));
		return (false);
	}
	if (channel->isUserInChannel(toKick) == false)
	{
		sendMessage(user, ERR_NOSUCKNICK, (command[2] + " :no such nick"));
		return (false);
	}
	if (channel->isUserInChannel(user) == false) 
	{
		sendMessage(user, ERR_NOTONCHANNEL, (command[1] + " :You're not on that channel"));
		return (false);
	}
	return (true);
}

void	Server::help(User *user)
{
	std::string msg = "\nBasic commands ->\n"
		"/INFO                              display your information\n"
		"/NICK [nick name]                  change your nickname\n"
		"/USER [user name]                  change your login\n"
		"/JOIN [channel]                    join channel\n"
		"/PART                              leave current channel\n"
		"/QUIT                              quit IRC\n"
		"/WHO                               list of users in current channel\n"
		"/MSG [nick name] [msg]             submit msg to login\n"
		"/LIST                              list of channel\n"
		"[msg]                              send msg to current channel";
	sendMessage(user, NOCODE, msg);
}

void	Server::info(User *user, std::vector<std::string> /*command*/)
{
	std::string msg = "\nYour username is : " + user->getUsername() +
		"\nYour nickname is : " + user->getNick() +
		"\nYour hostname is : " + user->getHostname() +
		"\nYour realname is : " + user->getReal();
	sendMessage(user, NOCODE, msg);
	help(user);
}
