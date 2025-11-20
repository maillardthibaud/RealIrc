/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:50:13 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/09/04 10:40:30 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos)
		return ("");
	size_t last = str.find_last_not_of(' ');
	return (str.substr(first, (last - first + 1)));
}

std::string reduce_spaces(const std::string &str) 
{
	std::string result;
	bool in_space = false;
	for (size_t i = 0; i < str.length(); ++i) 
	{
		if (std::isspace(str[i])) 
		{
			if (!in_space)
			{
				result += ' ';
				in_space = true;
			}
		}
		else 
		{
			result += str[i];
			in_space = false;
		}
	}
	return (result);
}

std::string trim_and_reduce_spaces(const std::string& str)
{
	std::string result;
	result = trim(str);
	result = reduce_spaces(result);
	return (result);
}

std::vector<std::string>	parsingIntoVector(std::string input)
{
	std::vector<std::string> command;
	input = trim_and_reduce_spaces(input);
	size_t start = 0;
	size_t end = input.find(" ");
	while (end != std::string::npos)
	{
		command.push_back(input.substr(start, end - start));
		start = end + 1;
		end = input.find(" ", start);
	}
	command.push_back(input.substr(start));
	return (command);
}

std::string	timestamp()
{
	std::string real_time;
	std::time_t now = std::time(0);
	std::string createTime = toStr(now);
	return (createTime);
}

void	trimTrailingSpace(std::string &str)
{
	if (!str.empty())
		str.erase(str.size() - 1);
}