/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdussert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 16:09:55 by jdussert          #+#    #+#             */
/*   Updated: 2021/12/15 16:09:56 by jdussert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

void		ft_parseCGI(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab)
{
	if (location->scriptName != "" || location->extension != "")
		ft_error(serverTab, "CGI parameters: duplicate key.");

	int		len = 0;
	size_t	pos = begin->find("cgi");
	ft_check_bad_char(0, pos, begin, "CGI", serverTab);
	pos += 3;
	ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	location->extension = begin->substr(pos, len).c_str();
	pos += len;
	len = 0;
	ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	location->scriptName = begin->substr(pos, len).c_str();
	ft_checkEndOfLine(begin, pos + len, serverTab, "CGI parameters");

	return;
}
