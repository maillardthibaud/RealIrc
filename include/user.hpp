#ifndef USER_HPP
#define USER_HPP

#include "main.hpp"

class Server;
class Channel;

class User
{
private:

	std::string			_realname;
	std::string			_username;
	std::string			_nickname;
	std::string			_hostname;
	int					_fd;
	std::string			_partialMessage;
	bool				_checkPass;
	

public:

	User();
	~User();

	bool				deleteMe;

	std::string			getUsername(void);
	std::string			getNick(void);
	std::string			getHostname(void);
	std::string			getReal(void);
	int					getFd(void);
	std::string			getPartialMessage(void);
	bool				getCheckPass(void);

	void				setNickname(std::string nickname);
	void				setFd(int fd);
	void				setPartialMessage(std::string str);
	void				setCheckPass(bool pass);
	void				setUsername(std::string username);
	void				setRealname(std::string realname);
	void				setHostname(std::string hostname);

	void				clearPartialMessage();
	bool				checkUserInfo();
};

#endif
