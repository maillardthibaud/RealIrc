#include "../../include/main.hpp"

void	Server::initServer()
{
	_connection.sa.sin_family = AF_INET;
	_connection.sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	_connection.sa.sin_port = htons(atoi(_port.c_str()));
	std::cout << "Server initialized with IP: 127.0.0.1 and PORT: " << _port << std::endl;
}

void	Server::initSocket()
{
	_connection.socketFd = socket(_connection.sa.sin_family, SOCK_STREAM, 0);
	if (_connection.socketFd == -1)
	{
		std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
		initError ex("Failed to create socket file descriptor");
		throw ex;
	}
}

void	Server::initFcntl()
{
	if (fcntl(_connection.socketFd, F_SETFL, O_NONBLOCK) == -1)
	{
		initError ex("fcntl");
		throw ex;
	}
}

void	Server::initBind()
{
	int opt = 1;
	setsockopt(_connection.socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	_connection.status = bind(_connection.socketFd, (struct sockaddr *)&_connection.sa, sizeof(_connection.sa));
	if (_connection.status != 0)
	{
		std::cerr << "Error on bind: " << strerror(errno) << std::endl;
		shutdown_signal = true;
		close(_connection.socketFd);
		_connection.socketFd = -1;
		initError ex("Failed to bind socket to address");
		throw ex;
	}
}

void	Server::initListen()
{
	_connection.status = listen(_connection.socketFd, 20);
	if (_connection.status != 0)
	{
		std::cerr << "Error on listen: " << strerror(errno) << std::endl;
		shutdown_signal = true;
		close(_connection.socketFd);
		_connection.socketFd = -1;
		initError ex("Failed to listen on socket");
		throw ex;
	}
}

void	Server::init()
{
	std::cout << "[ SERVER INIT ] Initializing server..." << std::endl;
	initServer();
	std::cout << "[ SERVER INIT ] Server initialized successfully." << std::endl;

	std::cout << "[ SOCKET INIT ] Initializing socket..." << std::endl;
	initSocket();
	std::cout << "[ SOCKET INIT ] Socket initialized successfully." << std::endl;

	std::cout << "[ FCNTL INIT ] fcntl Initializing..." << std::endl;
	initFcntl();
	std::cout << "[ FCNTL INIT ] Fcntl initialized successfully." << std::endl;

	std::cout << "[ BIND INIT ] Binding socket to address and port..." << std::endl;
	initBind();
	std::cout << "[ BIND INIT ] Socket bound successfully." << std::endl;

	std::cout << "[ SERVER RUN ] Server is running." << std::endl;
	std::cout << "Listening on localhost, PORT: " << _port << std::endl;

	std::cout << "[ LISTEN INIT ] Preparing to listen for incoming connections..." << std::endl;
	initListen();
	std::cout << "[ LISTEN INIT ] Now listening for incoming connections." << std::endl;

	_connection.addrSize = sizeof(_connection.clientAddr);
}
