/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 13:07:04 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/09/05 08:24:26 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <climits>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring> 
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

#include "server.hpp"
#include "user.hpp"
#include "channel.hpp"
#include "utils.hpp"

#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"

#define NOCODE "-42"

#define RPL_WHOISUSER "311"
#define RPL_WHOISOPERATOR "313"
#define RPL_ENDOFWHO "315"
#define RPL_WHOISIDLE "317"
#define RPL_ENDOFWHOIS "318"
#define RPL_WHOISCHANNELS "319"

#define RPL_LIST "321"
#define RPL_LISTEND "323"
#define RPL_CHANNELMODEIS "324"
#define RPL_CREATIONTIME "329"

#define RPL_NOTOPIC "331"
#define RPL_TOPIC "332"
#define RPL_TOPICWHOTIME "333"

#define RPL_INVITING "341"


#define RPL_NAMREPLY "353"

#define RPL_WHOREPLY "352"
#define RPL_NAMREPLY "353"

#define RPL_ENDOFNAMES "366"

#define RPL_ENDOFMOTD "376"

#define ERR_NOSUCKNICK "401"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_NOTEXTTOSEND "412"

#define ERR_UNKNOWNCOMMAND "421"

#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"

#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL "442"
#define ERR_USERONCHANNEL "443"

#define ERR_NEEDMOREPARAMS "461"
#define ERR_PASSWDMISMATCH "464"

#define ERR_CHANNELISFULL "471"
#define ERR_UNKNOWNMODE "472"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BADCHANNELKEY "475"

#define ERR_CHANOPRIVSNEEDED "482"

#define BUFFER_SIZE 512

#endif
