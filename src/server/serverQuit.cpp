#include "../../include/main.hpp"

void	Server::shutdown()
{
	removeAllUsersFromServer();
	std::cout << "Server exited gracefully" << std::endl;
}

void	Server::quit(User * user, std::vector<std::string> /*command*/)
{
	if (!user)
	{
		std::cerr << "Error: Invalid user pointer" << std::endl;
		return ;
	}
	removeUserFromAllChannels(user);
	if (user)
		removeUserFromServer(user);
}

void Server::removeUserFromServer(User *user)
{
	int fd = user->getFd();

	std::cout << "Now closing client : " << user->getNick() << std::endl;
	deleteUser(fd);
	removeClientFromPoll(fd);
	if (close(fd) == -1)
		std::cerr << "Error on close : " << strerror(errno) << std::endl;
}

void Server::removeUserFromAllChannels(User *user)
{
	std::vector<std::string> userChannel;
	std::vector<Channel*> channelsToLeave;

	userChannel.push_back(user->getNick());

	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		channelsToLeave.push_back(*it);
	}

	for (std::vector<Channel*>::iterator it = channelsToLeave.begin(); it != channelsToLeave.end(); ++it)
	{
		userChannel.push_back((*it)->getName());
		leave(user, userChannel);
		userChannel.pop_back();
	}

	userChannel.clear();
}

void	Server::removeAllUsersFromServer()
{
	while (!_fdUsers.empty())
	{
		std::map<int, User*>::iterator it = _fdUsers.begin();
		removeUserFromAllChannels(it->second);
		removeUserFromServer(it->second);
	}
}
