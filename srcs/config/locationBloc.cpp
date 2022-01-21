/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationBloc.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jehannedussert <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 13:52:29 by jehannedu         #+#    #+#             */
/*   Updated: 2022/01/15 13:52:31 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

void    ft_redirection(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab)
{
	if (location->redir.size())
		ft_error(serverTab, "Location redirection: duplicate key.");

	short int	ret = 0;
	std::string	path = "";
	int			len = 0;
	size_t		pos = begin->find("return");

	ft_check_bad_char(0, pos, begin, "Location redirection", serverTab);
	pos += 6;
	ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != '/')
		len++;
	if (!(ret = atoi(begin->substr(pos, len).c_str())))
		ft_error(serverTab, "Location redirection: wrong code.");
	pos += len;
	ft_skip_isspace(begin, &pos, serverTab);
	len = 0;
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	if (!len)
		ft_error(serverTab, "Location redirection: incorrect path.");
	path = begin->substr(pos, len);
	ft_checkEndOfLine(begin, pos + len, serverTab, "Location redirection");
	location->redir.insert(std::pair<short int, std::string>(ret, path));

	return;
}

void    ft_methodsFound(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab)
{
	if (location->methods.size())
		ft_error(serverTab, "Methods: duplicate key.");

	std::string	method;
    size_t		pos = begin->find("methods");
    size_t		len;

	ft_check_bad_char(0, pos, begin, "Accepted methods", serverTab);
	pos += 7;
	while ((*begin)[pos])
	{
		len = 0;
		if ((*begin)[pos] == ';')
			return ft_check_bad_char(++pos, begin->length(), begin, "Accepted methods", serverTab);
		ft_skip_isspace(begin, &pos, serverTab);
		while ((*begin)[pos + len])
		{	
			if ((*begin)[pos + len] == ';' || isspace((*begin)[pos + len]))
				break;
			len++;
		}
		method = begin->substr(pos, len);
		if (method == "")
			break;
		if (method != "GET" && method != "POST" && method != "DELETE")
			ft_error(serverTab, "Wrong accepted methods [" + method + "]");
		location->methods.push_back(method);
		pos += len;
	}
	ft_checkEndOfLine(begin, pos, serverTab, "accepted methods");
}

void    ft_locationRoot(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab)
{
	if (location->root != "")
		ft_error(serverTab, "Location root: duplicate key.");

	int		len = 0;
	size_t	pos = begin->find("root");
	struct	stat s;

	ft_check_bad_char(0, pos, begin, "Location root", serverTab);
	pos += 4;
	ft_skip_isspace(begin, &pos, serverTab);
	if ((*begin)[pos] != '/')
		ft_error(serverTab, "Wrong location root.");
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	location->root = begin->substr(pos, len);
	if (location->root != "" && stat(location->root.c_str(), &s) == -1)
		ft_error(serverTab, "Wrong location root : directory does not exist [" + location->root + "]");
	if (location->root[location->root.length() - 1] != '/')
		location->root += '/';
	ft_checkEndOfLine(begin, pos + len, serverTab, "Location root");

	return;
}

void    ft_locationAutoindex(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab)
{
	if (location->autoindex)
		ft_error(serverTab, "Location autoindex: duplicate key.");

    int		len = 0;
	size_t	pos = begin->find("autoindex");

	ft_check_bad_char(0, pos, begin, "Auto-index", serverTab);
	pos += 9;
	ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	if (!len)
		ft_error(serverTab, "Autoindex: missing value.");
	else if ((begin->substr(pos, len)) == "on")
		location->autoindex = 1;
	else if ((begin->substr(pos, len)) != "off")
		ft_error(serverTab, "Wrong autoindex [" + (begin->substr(pos, len)) + "]");
	if ((*begin)[pos + len] == ';')
		return ft_check_bad_char(++pos + len, begin->length(), begin, "Auto-index", serverTab);
	ft_checkEndOfLine(begin, pos + len, serverTab, "location auto-index");

	return;
}

void    ft_locationIndex(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab)
{
	if (location->index != "")
		ft_error(serverTab, "Location index: duplicate key.");

    int			len = 0;
	size_t		pos = begin->find("index");

	ft_check_bad_char(0, pos, begin, "Location index", serverTab);
	pos += 5;
	ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	location->index = begin->substr(pos, len);
	ft_checkEndOfLine(begin, pos + len, serverTab, "Location index");
	
	return;
}

void    ft_locationPath(size_t *pos, std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab)
{
	if (location->path != "")
		ft_error(serverTab, "Location path: duplicate key.");

    size_t	len = 0;

    if ((*begin)[*pos] != '/')
		ft_error(serverTab, "Location: invalid path.");
	while (!isspace((*begin)[*pos + len]))
		len++;
	location->path = begin->substr(*pos, len);
	*pos += len;
	ft_skip_isspace(begin, pos, serverTab);
	if ((*begin)[*pos] != '{')
		ft_error(serverTab, "Location: invalid path.");
	ft_check_bad_char(++(*pos), begin->length(), begin, "Location", serverTab);
}
