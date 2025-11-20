#include "../include/main.hpp"

extern Server	*ircservPtr;

void	signalHandler(int signum)
{
	if (signum != SIGPIPE)
		ircservPtr->shutdown_signal = true;
	else
		std::cerr << "Error: Borken pipe." << std::endl;
}

void	setupSignalHandling()
{
	struct sigaction	sa;

	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	signal(SIGPIPE, SIG_IGN);
	if (sigaction(SIGHUP, &sa, NULL) == -1)
	{
		std::cerr << "Error setting up SIGHUP handler" << std::endl;
		exit(1);
	}
	if (sigaction(SIGTERM, &sa, NULL) == -1)
	{
		std::cerr << "Error setting up SIGTERM handler" << std::endl;
		exit(1);
	}
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		std::cerr << "Error setting up SIGINT handler" << std::endl;
		exit(1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		std::cerr << "Error setting up SIGQUIT handler" << std::endl;
		exit(1);
	}
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		std::cerr << "Error setting up SIGUSR1 handler" << std::endl;
		exit(1);
	}
}

