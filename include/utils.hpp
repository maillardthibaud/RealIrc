/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaillar <tmaillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:54:45 by botyonthesk       #+#    #+#             */
/*   Updated: 2024/09/03 08:13:47 by tmaillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "main.hpp"

std::string trim_and_reduce_spaces(const std::string& str);
std::string reduce_spaces(const std::string &str);
std::string trim(const std::string &str);
std::string splitBuffer(std::string buffer);
std::string splitBuffer2(std::string buffer);
std::string	timestamp();
std::vector<std::string>	parsingIntoVector(std::string input);
void						trimTrailingSpace(std::string &str);

#endif