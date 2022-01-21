/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jehannedussert <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 13:53:34 by jehannedu         #+#    #+#             */
/*   Updated: 2022/01/15 13:53:35 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

int		ft_findKey(short int key, std::vector<short int> haystack)
{
	std::vector<short int>::const_iterator	ite = haystack.end();

	for (std::vector<short int>::const_iterator it = haystack.begin(); it != ite; it++)
		if (*it == key)
			return (FAILURE);
	return (SUCCESS);
}

std::string			ft_firstWord(std::vector<std::string>::iterator begin, std::vector<s_serverConfig> &serverTab)
{
	std::string		param = "";
	size_t			pos = 0, len = 0;

	if (isspace((*begin)[pos]))
		ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin) == "" || (*begin)[pos] == '\n')
		++begin;
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	param = begin->substr(pos, len);

	return param;
}

void	ft_check_bad_char(size_t idx, size_t end, std::vector<std::string>::iterator begin, std::string err, std::vector<s_serverConfig> &serverTab)
{
	std::string	msg = err;
	msg += ": bad char detected.";

	while (idx < end)
	{
		if (!isprint((*begin)[idx]))
			return;
		if (!isspace((*begin)[idx++]))
			ft_error(serverTab, msg);
	}
}

void	ft_skip_isspace(std::vector<std::string>::iterator begin, size_t *pos, std::vector<s_serverConfig> &serverTab)
{
	if (!isspace((*begin)[*pos]))
		ft_error(serverTab, "Wrong configuration file: missing spaces after keyword.");
    while ((*begin)[*pos] && isspace((*begin)[*pos]))
		(*pos)++;
}

void	ft_check_spaces(std::vector<std::string>::iterator begin, size_t *pos, std::vector<s_serverConfig> &serverTab)
{
	while ((*begin)[*pos])
	{
		if (isspace((*begin)[*pos]))
			ft_skip_isspace(begin, pos, serverTab);
		if (isprint((*begin)[*pos]))
			ft_error(serverTab, "Wrong configuration file.");
		else if (!isprint((*begin)[*pos]))
			return ;
	}
}

bool	ft_checkExtension(std::string str, std::string toFind)
{
	size_t len = str.length() + 2;
	std::string	extension = str.substr(str.find_last_of(".") + 1);
	
	if (len < toFind.length() || extension.compare(toFind))
		return FALSE;
	return TRUE;
}
