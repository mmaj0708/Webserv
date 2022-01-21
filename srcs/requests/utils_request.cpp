/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 20:44:54 by vlugand-          #+#    #+#             */
/*   Updated: 2021/12/02 10:59:54 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

extern	s_global g_all;

bool	URI_is_trespassing(std::string URI)
{
	size_t		pos = 0;
	int			root = 0;
	std::string	tmp;

	while (pos < URI.size())
	{
		URI.erase(0, pos + 1);
		pos = URI.find_first_of('/');
		if (pos == std::string::npos)
			pos = URI.size();
		tmp = URI.substr(0, pos);
		if (tmp == "..")
			root--;
		else
			root++;
		if (root < 0)
			return (TRUE);
	}
	return (FALSE);
}

size_t		find_first_whitespace(const std::string s)
{
	size_t	pos;
	int		wsp[6] = {32, 9, 10, 11, 12, 13};
	int		i;

	for (i = 0; i < 6; i++)
	{
		if ((pos = s.find_first_of(wsp[i])) != std::string::npos)
			return (pos);
	}
	return (-1);
}

void		remove_first_ows(std::string & s)
{
	size_t	pos = 0;

	for (std::string::const_iterator it = s.cbegin() ; it != s.cend(); ++it)
	{
        if (*it != ' ' && *it != '\t')
			break ;
		pos++;
    }
	if (pos)
		s.erase(0, pos);	
}

void	str_toupper(std::string & s)
{
	for (std::string::iterator it = s.begin(); it != s.end(); it++)
		*it = std::toupper(*it);
	return ;
}

size_t	hex_str_to_int(std::string s)
{
	size_t x;
    std::stringstream ss;

    ss << std::hex << s;
    ss >> x;
    return (x);
}

size_t	find_existing_request(short int csocket)
{
	size_t	i;

	for (i = 0; i < g_all.pendingRequests.size(); i++)
	{
		if (csocket == g_all.pendingRequests[i].getClientSocket() &&
		g_all.pendingRequests[i].getParsingStatus() < 4 && g_all.pendingRequests[i].getParsingStatus() >= 0)
			return (i);
	}
	return (g_all.pendingRequests.size());
}
