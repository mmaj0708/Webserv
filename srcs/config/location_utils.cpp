/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location_utils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jehannedussert <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 13:52:06 by jehannedu         #+#    #+#             */
/*   Updated: 2022/01/15 13:52:09 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

int	ft_locInBrackets(std::vector<std::string>::iterator *idx, std::vector<std::string>::iterator *ite, std::vector<s_location> &locTab, std::vector<s_serverConfig> &serverTab)
{
	std::vector<std::string>::iterator begin;
	std::vector<std::string>::iterator end;
	std::vector<std::string>::iterator it = *idx;
	size_t found = (it)->find("location");

	if (found != std::string::npos)
	{
		size_t	b = 0;
		for (; it != *ite; it++)
		{
			for (size_t j = 0; j < it->length() && (*it)[j]; j++)
			{
				while ((*it)[j] && (*it)[j] != '{' && (*it)[j] != '}')
					j++;
				if ((*it)[j] == '{' && !b)
				{
					if (!b)
						begin = std::vector<std::string>::iterator(it);
					b++;
				}
				else if ((*it)[j] == '{' && b)
					throw std::logic_error("Location syntax error.");
				if (b > 1)
					throw std::logic_error("Location syntax error.");
				while ((*it)[j] && (*it)[j] != '{' && (*it)[j] != '}')
					j++;
				if ((*it)[j] == '}' && b)
				{
					if (!--b)
					{
						end = std::vector<std::string>::iterator(it);
						locTab.push_back(ft_newLocation(begin, end, serverTab));
						return (SUCCESS);
					}
				}
			}
			(*idx)++;
		}
	}
	return ft_error(serverTab, "Location block is not closed.");
}

int	ft_isInLocation(std::vector<std::string>::iterator *it, std::vector<std::string>::iterator ite, std::vector<s_location> &locTab, std::vector<s_serverConfig> &serverTab)
{
	size_t	pos;

	for (; *it != ite; (*it)++)
	{
		pos = 0;
		size_t found = (*it)->find("location");
		if (found != std::string::npos)
		{
			ft_check_bad_char(0, found, *it, "Location", serverTab);
			if (ft_locInBrackets(it, &ite, locTab, serverTab) == SUCCESS)
				return (SUCCESS);
			else
				return (FAILURE);
		}
		else if (*(*it) != "")
			ft_check_spaces(*it, &pos, serverTab);
	}
	return (SUCCESS);
}

void    ft_locationFound(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab)
{
    size_t		pos = begin->find("location");

	ft_check_bad_char(0, pos, begin, "Location", serverTab);
	pos += 8;
	ft_skip_isspace(begin, &pos, serverTab);
	ft_locationPath(&pos, begin, location, serverTab);
}
