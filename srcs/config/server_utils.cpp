/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jehannedussert <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 13:53:17 by jehannedu         #+#    #+#             */
/*   Updated: 2022/01/15 13:53:18 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

int	ft_isInBrackets(std::vector<s_serverConfig> &serverTab, std::vector<std::string>::iterator *idx, std::vector<std::string>::iterator *tabEnd)
{
	size_t	b = 0;
	size_t	pos;
	std::vector<std::string>::iterator it = *idx;
	std::vector<std::string>::iterator begin;
	std::vector<std::string>::iterator end;

	for (; it != *tabEnd; it++)
	{
		for (size_t j = 0; j < it->length(); j++)
		{
			if ((pos = it->find("server ")) != std::string::npos)
				j = pos + 7;
			while ((*it)[j] && isspace((*it)[j]))
				j++;
			if ((*it)[j] != '{' && !b)
				ft_error(serverTab, "Bad char detected.");
			else if ((*it)[j] == '{')
			{
				if (!b)
					begin = std::vector<std::string>::iterator(it);
				b++;
				ft_check_bad_char(++j, it->length(), it, "Bad char detected.", serverTab);
			}
			else if ((*it)[j] == '}' && b)
			{
				ft_check_bad_char(++j, it->length(), it, "Bad char detected.", serverTab);
				if (!--b)
				{
					end = std::vector<std::string>::iterator(it);
					!serverTab.size() ? serverTab.push_back(ft_newServer(++begin, end, true, serverTab)) : serverTab.push_back(ft_newServer(++begin, end, false, serverTab));			
					return (SUCCESS);
				}
			}
		}
		(*idx)++;
	}
	return ft_error(serverTab, "Server block is not closed.");
}

int	ft_isInServer(std::vector<std::string> tab, std::vector<s_serverConfig> &serverTab)
{
	size_t pos;
	std::vector<std::string>::iterator end = tab.end();
	
	for (std::vector<std::string>::iterator it = tab.begin(); it != end; it++)
	{
		pos = 0;
		size_t found = it->find("server");
		if (found != std::string::npos)
		{
			ft_check_bad_char(0, found, it, "Server", serverTab);
			if (ft_isInBrackets(serverTab, &it, &end) == SUCCESS)
				continue ;
			else
				return (FAILURE);
		}
		else if (*it != "")
			ft_check_spaces(it, &pos, serverTab);
	}
	return (SUCCESS);
}

void	ft_checkEndOfLine(std::vector<std::string>::iterator begin, size_t pos, std::vector<s_serverConfig> &serverTab, std::string param)
{
	std::string	err = "Wrong " + param + ".";

	if (isspace((*begin)[pos]))
		ft_skip_isspace(begin, &pos, serverTab);
	if ((*begin)[pos] == ';')
		return ft_check_bad_char(++pos, begin->length(), begin, param, serverTab);
	else
		ft_error(serverTab, err);
}
