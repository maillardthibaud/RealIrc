#include "../../include/main.hpp"

void	Server::sendForInfo(int clientFd, std::string code, std::string message)
{
	std::string msg;
	if (code == NOCODE)
		msg = message;   
	else
		msg = ":" + _name + " " + code + " " + message + "\r\n";
	sendToFd(clientFd, msg);

}

void	Server::SendSpeMsg(User * userId, User * toSend, std::string msg)
{
	std::string userMsg = ":" + userId->getNick() + "!" + userId->getUsername() + "@" + userId->getHostname() + " " + msg + "\r\n";
	if(!sendToFd(toSend->getFd(), userMsg))
		toSend->deleteMe = true;
}

void	Server::sendMessage(User * user, std::string numCode, std::string message)
{
	std::string userMsg;

	if (numCode != NOCODE)
		userMsg = ":" + _name + " " + numCode + " " + user->getNick() + " " + message + "\r\n";
	else
		userMsg = message + "\r\n";
	if (!sendToFd(user->getFd(), userMsg))
		user->deleteMe = true;
}

ssize_t 	Server::secureSend(int fd, const void *buffer, size_t length, int flags)
{
	if (fd < 0)
		return (-1);
	if (buffer == NULL || length == 0)
		return (-1);
	ssize_t totalSent = 0;
	ssize_t byteSend = 0;
	while (totalSent < static_cast<ssize_t>(length))
	{
		byteSend = send(fd, static_cast<const char*>(buffer) + totalSent, length - totalSent, flags);
		if (byteSend == -1) 
		{
            if (errno == EINTR) 
			{
                continue;
            } 
			else if (errno == EPIPE || errno == ECONNRESET)
			{
                std::cerr << "Erreur: Le client s'est déconnecté." << std::endl;
                return -1;
            }
			else
			{
                std::cerr << "Erreur lors de l'envoi: " << strerror(errno) << std::endl;
                return -1;
            }
        } else if (byteSend == 0) 
		{
            std::cerr << "Erreur: Le client s'est déconnecté." << std::endl;
            return -1;
        }
        totalSent += byteSend;
	}
	return (totalSent);
}


bool	Server::sendToFd(int clientFd, std::string message)
{
	ssize_t	bytesSend = 0;
	ssize_t msgSize = message.size();
	// bytesSend = secureSend(clientFd, message.c_str(), msgSize, 0);
	// if (bytesSend == -1)
	// {
	// 	std::cout << "Error on send" << std::endl;
	// 	close(clientFd);
	// 	// shutdown_signal = true;
	// }
	if (clientFd != -1)
		bytesSend = send(clientFd, message.c_str(), msgSize, MSG_NOSIGNAL);
	if (bytesSend == -1)
	{

		// deleteUser(clientFd);
		// removeClientFromPoll(clientFd);
		// std::cout << "Error on send" << std::endl;
		// close(clientFd);
		return (false);
	}
	else if (bytesSend != msgSize)
	{
		std::cout << "Only partial message received by client socket : " << clientFd
		<< ", bytes send = " << bytesSend << std::endl;
		return (false);
	}
	return (true);
}

void	Server::sendMsgToChanFromUser(Channel * channel, User * user, std::string input)
{
	std::string userMsg = ":";

	userMsg += user->getNick() + "!" + user->getUsername() + "@" + user->getHostname() + " " + input;

	std::vector<User*> usersInChannel = channel->getUsers();
	for (std::vector<User*>::iterator it = usersInChannel.begin(); it != usersInChannel.end(); ++it)
	{
		if ((*it) != user)
			sendMessage((*it), NOCODE, userMsg);
	}
}

void	Server::sendSpeMsgToAllUsersInChannelExceptOne(Channel *channel, User *user, std::string msg)
{
	std::vector<User*>	usersInChannel = channel->getUsers();

	for (std::vector<User*>::iterator it = usersInChannel.begin(); it != usersInChannel.end(); ++it)
	{
		if ((*it) != user)
			SendSpeMsg(user, (*it), msg);
	}
}

void	Server::sendSpeMsgToAllUsersInChannel(Channel *channel, User *user, std::string msg)
{
	std::vector<User*>	usersInChannel = channel->getUsers();

	for (std::vector<User*>::iterator it = usersInChannel.begin(); it != usersInChannel.end(); ++it)
		SendSpeMsg(user, (*it), msg);
}
