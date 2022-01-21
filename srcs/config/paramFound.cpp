/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paramFound.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jehannedussert <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 13:52:57 by jehannedu         #+#    #+#             */
/*   Updated: 2022/01/15 13:52:59 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

void	ft_maxBodyFound(std::vector<std::string>::iterator begin, s_serverConfig *newServer, std::vector<s_serverConfig> &serverTab)
{
	if (newServer->client_max_body_size != 1000000)
		ft_error(serverTab, "Client max body size: duplicate key.");
	
	int		len = 0;
	size_t	pos = begin->find("client_max_body_size");
	ft_check_bad_char(0, pos, begin, "Client max body size", serverTab);
	pos += 20;
	ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin)[pos + len] && isdigit((*begin)[pos + len]))
		len++;
	if (!(newServer->client_max_body_size = atoi((begin->substr(pos, len)).c_str())))
		ft_error(serverTab, "Max body size: wrong value [" + begin->substr(pos, 1) + "]");
	if (atoi((begin->substr(pos, len)).c_str()) > INT_MAX || atoi((begin->substr(pos, len)).c_str()) < 0)
		ft_error(serverTab, "Max body size: wrong value [" + begin->substr(pos, len) + "]");
	ft_checkEndOfLine(begin, pos + len, serverTab, "server client max body size");

	return;
}

void	ft_listenFound(std::vector<std::string>::iterator begin, s_serverConfig	*newServer, std::vector<s_serverConfig> &serverTab)
{
	if (newServer->port || !newServer->ip.empty())
		ft_error(serverTab, "Listen: duplicate key.");

	int		len = 0;
	size_t	pos = begin->find("listen");

	ft_check_bad_char(0, pos, begin, "Listen", serverTab);
	pos += 6;
	ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin)[pos + len] && (*begin)[pos + len] != ':')
		len++;
	if (begin->find(":") != std::string::npos)
		newServer->ip = begin->substr(pos, len);
	len = 0;
	pos = begin->find(":");
	if (pos == std::string::npos)
	{
		pos = begin->find("listen") + 6;
		if (!(isspace((*begin)[pos])))
			ft_error(serverTab, "Wrong port.");
		ft_skip_isspace(begin, &pos, serverTab);
		while (!isdigit((*begin)[pos + len]) && (*begin)[pos + len] != '-'
			&& (*begin)[pos + len] != ';' && (*begin)[pos + len])
			pos++;
	}
	else if ((*begin).find_last_of(":"))
		pos = (*begin).find_last_of(":") + 1;
	while ((*begin)[pos + len] && (isdigit((*begin)[pos + len]) || (*begin)[pos + len] == '-'))
		len++;
	if (!(newServer->port = atoi((begin->substr(pos, len)).c_str())) || (atoi((begin->substr(pos, len)).c_str())) < 0 
		|| (atoi((begin->substr(pos, len)).c_str())) > SHRT_MAX)
		ft_error(serverTab, "Port: wrong value [" + begin->substr(pos, len) + "]");
	ft_checkEndOfLine(begin, pos + len, serverTab, "listen");

	return;
}

void	ft_rootFound(std::vector<std::string>::iterator begin, s_serverConfig *newServer, std::vector<s_serverConfig> &serverTab)
{
	if (newServer->root != "")
		ft_error(serverTab, "Root: duplicate key.");

	struct	stat s;
	int		len = 0;
	size_t	pos = begin->find("root");
	
	ft_check_bad_char(0, pos, begin, "Root", serverTab);
	pos += 4;
	ft_skip_isspace(begin, &pos, serverTab);
	if ((*begin)[pos] != '/')
		ft_error(serverTab, "Server root: bad char detected.");
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	newServer->root = begin->substr(pos, len);
	if (newServer->root[newServer->root.length() - 1] != '/')
		newServer->root += '/';
	if (stat(newServer->root.c_str(), &s) == -1)
		ft_error(serverTab, "Server root: directory does not exist [" + newServer->root + "]");
	ft_checkEndOfLine(begin, pos + len, serverTab, "server root");

	return;
}

void	ft_serverNameFound(std::vector<std::string>::iterator begin, s_serverConfig *newServer, std::vector<s_serverConfig> &serverTab)
{
	if (newServer->server_name != "")
		ft_error(serverTab, "Server name: duplicate key.");

	int		len = 0;
	size_t	pos = begin->find("server_name");
	
	ft_check_bad_char(0, pos, begin, "Server name", serverTab);
	pos += 11;
	ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	newServer->server_name = begin->substr(pos, len);
	ft_checkEndOfLine(begin, pos + len, serverTab, "server name");

	return;
}

void	ft_errorPageFound(std::vector<std::string>::iterator begin, s_serverConfig *newServer, std::vector<s_serverConfig> &serverTab)
{
	short int	codeErr = 0;
	std::string	pathErr = "";
	int			len = 0;
	size_t		pos = begin->find("error_page");

	ft_check_bad_char(0, pos, begin, "Error page", serverTab);
	pos += 10;
	ft_skip_isspace(begin, &pos, serverTab);
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != '/')
		len++;
	if (!(codeErr = atoi(begin->substr(pos, len).c_str())) || (atoi((begin->substr(pos, len)).c_str())) < SHRT_MIN
		|| (atoi((begin->substr(pos, len)).c_str())) > SHRT_MAX)
		ft_error(serverTab, "Error pages: wrong value [" + begin->substr(pos, len) + "]");	
	if (newServer->error_page.count(codeErr))
		ft_error(serverTab, "Error pages: duplicate error page code [" + begin->substr(pos, len) + "]");
	pos += len;
	ft_skip_isspace(begin, &pos, serverTab);
	len = 0;
	while ((*begin)[pos + len] && !isspace((*begin)[pos + len]) && (*begin)[pos + len] != ';')
		len++;
	if (!len)
		ft_error(serverTab, "Error pages: incorrect path.");
	pathErr = begin->substr(pos, len);
	if (pathErr.find_first_of('/') == 0)
		pathErr.erase(0, 1);
	ft_checkEndOfLine(begin, pos + len, serverTab, "server error page");
	newServer->error_page.insert(std::pair<short int, std::string>(codeErr, pathErr));
		
	return;
}
