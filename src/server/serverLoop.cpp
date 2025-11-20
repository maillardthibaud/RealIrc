#include "../../include/main.hpp"

void Server::run()
{
	try
	{
		init();
		waitingClient();
	}
	catch (const std::exception &e)
	{
		std::cerr << "[ ERROR ] " << e.what() << std::endl;
	}
}

void Server::waitingClient()
{
	initializePollFds();

	while (!shutdown_signal)
	{
		int pollCount = poll(_fds, _nfds, -1);
		if (pollCount == -1)
		{
			if (errno == EINTR)
				continue;
			handlePollError();
		}
		processPollEvents();
	}
}

void Server::initializePollFds()
{
	_nfds = 1;
	_fds[0].fd = _connection.socketFd;
	_fds[0].events = POLLIN;

	for (int i = 1; i < MAXCLIENT; ++i)
		_fds[i].fd = -1;
}

void Server::handlePollError()
{
	if (shutdown_signal == false)
	{
		std::cerr << "Error on poll: " << strerror(errno) << std::endl;
		initError ex("poll");
		throw ex;
	}
}

void Server::processPollEvents()
{
	for(std::map<int, User*>::iterator it = _fdUsers.begin(); it != _fdUsers.end();)
	{
		if (it->second->deleteMe == true)
		{
			removeUserFromAllChannels(it->second);
			std::map<int, User*>::iterator toRemove = it++;
			removeUserFromServer(toRemove->second);
		}
		else
			++it;
	}
	for (int i = 0; i < _nfds; ++i)
	{
		if (_fds[i].revents & POLLIN)
		{
			if (_fds[i].fd == _connection.socketFd)
				acceptNewClient();
			else
				readingClient(getUser(_fds[i].fd));
		}
	}
}

void Server::acceptNewClient()
{
	int new_fd = accept(_connection.socketFd, (struct sockaddr *) &_connection.clientAddr, (socklen_t *) &_connection.addrSize);

	if (new_fd == -1)
	{
		perror("accept");
		return;
	}
	handleClient(new_fd);
}

void Server::handleClient(int clientFd)
{
	int availableIndex = findAvailableIndex();
	if (availableIndex == -1)
	{
		std::cerr << "No available slot for new client\n";
		close(clientFd);
		return;
	}
	addClientToPoll(availableIndex, clientFd);
	createUserForClient(clientFd);
}

int Server::findAvailableIndex()
{
	for (int i = 1; i < MAXCLIENT; ++i)
	{
		if (_fds[i].fd == -1)
			return i;
	}
	return -1;
}

void Server::addClientToPoll(int index, int clientFd)
{
	_fds[index].fd = clientFd;
	_fds[index].events = POLLIN | POLLOUT;
	if (index >= _nfds)
		_nfds = index + 1;
}

void Server::createUserForClient(int clientFd)
{
	User* newUser = new User();
	newUser->setFd(clientFd);
	_fdUsers.insert(std::make_pair(clientFd, newUser));
}

void Server::readingClient(User *user)
{
	int clientFd = user->getFd();
	char buffer[BUFFER_SIZE];
	int bytesRead = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);

	if (bytesRead <= 0)
		handleClientDisconnection(clientFd, bytesRead);
	else
	{
		buffer[bytesRead] = '\0';
		manageBuff(buffer, user);
	}
}

void Server::handleClientDisconnection(int clientFd, int bytesRead)
{
	if (bytesRead == 0)
		std::cout << "Client disconnected: " << clientFd << std::endl;
	else
		std::cerr << "Error on recv: " << strerror(errno) << std::endl;

	// removeUserFromAllChannels(getUser(clientFd));
	// removeUserFromServer(getUser(clientFd));
	getUser(clientFd)->deleteMe = true;
}

void Server::removeClientFromPoll(int clientFd)
{
	for (int i = 0; i < _nfds; ++i)
	{
		if (_fds[i].fd == clientFd)
		{
			_fds[i].fd = -1;
			_fds[i].revents = 0;
			_fds[i].events = 0;
			break;
		}
	}
}

User *Server::getUser(int fd)
{
	std::map<int, User *>::iterator it = _fdUsers.find(fd);
	if (it != _fdUsers.end())
		return (it->second);
	return (NULL);
}

void Server::deleteUser(int fd)
{
	std::map<int, User *>::iterator it = _fdUsers.find(fd);
	if (it != _fdUsers.end())
	{
		delete it->second;
		_fdUsers.erase(it);
		_nbClient--;
	}
}

User *Server::getUser(std::string nickname)
{
	for (std::map<int, User *>::iterator it = _fdUsers.begin(); it != _fdUsers.end(); ++it)
	{
		if (it->second->getNick() == nickname)
			return (it->second);
	}
	return (NULL);
}
