#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "main.hpp"

class User;
class Server;

class Channel
{
	private:
			std::string			_name;
			int					_nbUsers;
			std::vector<User*>	_users;
			std::vector<User*>	_invites;
			std::vector<User*>	_operators;
			bool				_isInvitOnly;
			bool				_topicForOpOnly;
			bool				_onPassWordOnly;
			bool				_isMaxUsers;
			int					_maxUsers;
			std::string			_password;
			std::string			_creator;
		
	public:
	
			std::string			topic;
			std::string			createTime;
			
			Channel(User * user, std::string name);
			~Channel();
			
			bool				isUserInChannel(User *user);
			bool				isOp(User *user);
			bool				isPassCorrect(const std::string &pass);
			bool				isUserInvited(User *user) const;
			std::string			whoList();

			std::string			getName(void);
			std::string			getCreator(void);
			std::vector<User*>	getUsers(void);
			std::vector<User*>	getOperators(void);
			int					getNbUser(void);
			int					getMaxUsers(void);
			bool				getIsInvit(void);
			bool				getTopicOpOnly(void);
			bool				getOnPassOnly(void);
			bool				getIsMaxUsers(void);
					
			void				setIsMaxUsers(bool value);
			void				setMaxUsers(int nb);
			void				setNbUser(int x);
			void				setUser(User * user);
			void				setOperator(User *user);
			void				setIsInvitOnly(bool value);
			void				setTopicForOpOnly(bool value);
			void				setOnPasswordOnly(bool value);
			void				setPassword(std::string password);
			void				setInvite(User *user);

			void				removeUser(const std::string &nick);
			void				removeUser(User *user);
			void				delInvite(User *user);
			void				delOperator(User *user);

};

#endif
