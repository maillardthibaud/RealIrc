#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

#define MAXCLIENT 10
#define MAXCHANNEL 3
#define PORT 6667


struct ServerConnection
{
	struct sockaddr_in		sa;
	struct sockaddr_storage	clientAddr;
	socklen_t				addrSize;
	int						socketFd;
	int						status;
	ServerConnection()
		: sa(), clientAddr(), addrSize(0), socketFd(-1), status(0) {};
};

class User;
class Channel;
class Server;

typedef void (Server::*ServerFunction)(User *, std::vector<std::string>);
typedef void (Server::*ModeFunction)(Channel *, User *user, std::vector<std::string>);

class Server
{
private:

	std::map<std::string, ServerFunction> _commandMap;
	std::map<std::string, ModeFunction> _modeMap;
	struct ServerConnection		_connection;
	struct pollfd				_fds[MAXCLIENT];
	int							_nfds;
	std::string					_port;
	std::string					_password;
	std::string					_createTime;
	int							_nbClient;
	int							_nbChannel;
	std::map<int, User*>		_fdUsers;
	std::vector<Channel*>		_channels;
	std::string					_name;
	bool						_isValidMode;

public:
 
	bool						shutdown_signal;

	Server(std::string port, std::string password);
	~Server();

	void						run(void);
	void						shutdown();

	//----------SERVER INIT----------
	void						init(void);
	void						initServer(void);
	void						initSocket(void);
	void						initFcntl(void);
	void						initBind(void);
	void						initListen(void);
    void						initializeCommandMap();
	void						initializeModeMap();

	//----------SERVER POLL----------
	
	void						initializePollFds();
	void						waitingClient(void);
	void						handlePollError();
	void						processPollEvents();

	ssize_t 					secureSend(int fd, const void *buffer, size_t length, int flags);
	void						readingClient(User *user);
	void						handleClientDisconnection(int clientFd, int bytesRead);
	void						removeClientFromPoll(int clientFd);
	void						manageBuff(std::string input, User *user);
	bool						manageUser(User *user, std::vector<std::string> command);
	int							findAvailableIndex();
	void						addClientToPoll(int index, int clientFd);

	//----------SERVER AUTH----------
	void						acceptNewClient();
	void						handleClient(int clientFd);
	bool						checkPassword(User *user, std::vector<std::string> command);
	bool						isValidUsername(std::string username);
	bool						isValidNickname(std::string nickname);
	// bool						checkMode(void);
	bool						checkNick(User *user, std::string nickname);
	void						createUserForClient(int clientFd);

	//----------SERVER SEND----------
	// void						infoClient(User *user);
	void						sendForInfo(int clientFd, std::string code, std::string message);
	void						sendMessage(User *user, std::string numCode, std::string message);
	void						welcome(User *user);
	bool						sendToFd(int clientFd, std::string message);
	void						SendSpeMsg(User *user, User * toSend, std::string msg);
	void						sendMsgToChanFromUser(Channel * channel, User *user, std::string input);
	void						sendSpeMsgToAllUsersInChannelExceptOne(Channel *channel, User *user, std::string msg);
	void						sendSpeMsgToAllUsersInChannel(Channel *channel, User *user, std::string msg);
	void						printLoginList(void);
	bool						isValidChannel(std::string name);

	std::string					getUsersInChannelStr(Channel * curr);

	// std::vector<std::string>	getCommand(void);

	//----------SERVER GETTERS----------
	User						*getUser(int fd);
	User						*getUser(std::string nickname);
	Channel*					getChannel(std::string name);
	void						addUserInfo(User *user, std::vector<std::string> info);

	//----------SERVER CLEANUP----------
	void						removeUserFromAllChannels(User *user);
	void						removeUserFromServer(User *user);
	void						removeAllUsersFromServer();
	void						deleteChannel(Channel *channel);
	void						deleteUser(int fd);

	//----------IRC COMMANDS----------
	void						manageInput(User *user, std::vector<std::string> command);
	void						nick(User *user, std::vector<std::string> command);
	void						userName(User *user, std::vector<std::string> command);
	void						join(User *user, std::vector<std::string> command);
	void						msg(User *user, std::vector<std::string> command);
	void						kick(User *user, std::vector<std::string> command);
	void						invite(User *user, std::vector<std::string> command);
	void						defTopic(User *user, std::vector<std::string> command);
	void						mode(User *user, std::vector<std::string> command);
	void						ping(User *user, std::vector<std::string> command);
	void						leave(User *user, std::vector<std::string> command);
	void						who(User *user, std::vector<std::string> command);
	void						whois(User *user, std::vector<std::string> command);
	void						list(User *user, std::vector<std::string> command);
	void						quit(User *user, std::vector<std::string> command);
	void						help(User *user);
	void						info(User *user, std::vector<std::string> command);

	// bool						checkOperator(User *user, std::string channelName);
	bool						checkUserChannelList(User *user, std::string nickname, std::string channel);
	bool						checkKickInfo(User *user, std::vector<std::string> command);
	// bool						checkKickInfo(std::string channel, User *user, User *toKick);
	void						joinChannel(User *user, std::vector<std::string> command, std::string name);
	bool						checkJoinChannel(User *user, Channel * channels, std::vector<std::string> command);
	bool						checkInvit(User *user, std::string channelName);
	bool						isValidArgv(User *user, std::string attribut, std::vector<std::string> command);
	bool						isValidModeL(User *user, std::string attribut, std::vector<std::string> command);
	// bool						isValidModeO(User *user, std::string attribut, std::vector<std::string> command);
	bool						isValidModeO(User *user, std::vector<std::string> command);
	bool						isValidModeK(User *user, std::string attribut, std::vector<std::string> command);
	bool						isValidAttribut(std::string attribute);
	void						createNewChannel(User *user, std::string name);
	void						channelMsg(User *user, std::vector<std::string> command);
	void						privateMsg(User *user, std::vector<std::string> command);
	int							privOrChan(std::string input);
	bool						checkTopicCommand(User *user, Channel * channelId, std::string channel);
	std::string					mergeCommand(std::vector<std::string> command);
	bool						checkInviteCommand(User *user, std::string nickTo, std::string channel);
	bool						isValidChannelName(std::string name);
	bool						checkModeCommand(User* user, std::vector<std::string> command);

	bool            			applyMode(User * user, Channel * channel, std::vector<std::string> command);
	void            			delUserN(int idx);
	
	void            			modeI(Channel * channelId, User *user, std::vector<std::string> command);
	void            			modeT(Channel * channelId, User *user, std::vector<std::string> command);
	void            			modeK(Channel * channelId, User *user, std::vector<std::string> command);
	void            			modeO(Channel * channelId, User *user, std::vector<std::string> command);
	void            			modeL(Channel * channelId, User *user, std::vector<std::string> command);

	std::string					getUserChannels(User *user);

	class initError : public std::exception
	{
	public :
		explicit initError(const std::string& error);
		virtual const char* what() const throw();

	private :
		char	_error[100];
	};
};

template <typename T>	std::string toStr(T tmp)
{
	std::ostringstream out;
	out << tmp;
	return out.str();
}

#endif
