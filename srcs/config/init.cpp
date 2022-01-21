/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jehannedussert <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 13:51:45 by jehannedu         #+#    #+#             */
/*   Updated: 2022/01/15 13:51:49 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

int		ft_findKey(std::string key, std::vector<s_sock_setup> s_sock)
{
	std::vector<s_sock_setup>::const_iterator	ite = s_sock.end();

	for (std::vector<s_sock_setup>::const_iterator it = s_sock.begin(); it != ite; it++)
		if (it->ip == key)
			return (FAILURE);
	return (SUCCESS);
}

void	ft_fill_portTab(std::vector<s_serverConfig> *serverTab, std::vector<short int> *portTab)
{
	if (!serverTab->size())
		return ;
	std::vector<s_serverConfig>::const_iterator	ite = serverTab->end();
	for (std::vector<s_serverConfig>::const_iterator it = serverTab->begin(); it != ite; it++)
		if (ft_findKey(it->port, *portTab) == SUCCESS)
			portTab->push_back(it->port);
}

void    ft_fillSocketSetup(std::vector<s_serverConfig> const *serverTab, std::vector<s_sock_setup> *s_sock)
{
	std::vector<s_serverConfig>::const_iterator	it = serverTab->begin();
	std::vector<s_serverConfig>::const_iterator	ite = serverTab->end();
	s_sock_setup								newSock;

	for (; it != ite; it++)
	{
		if (ft_findKey(it->ip, *s_sock) == FAILURE)
		{	
			for (std::vector<s_sock_setup>::iterator itP = s_sock->begin(); itP != s_sock->end(); itP++)
				if (itP->ip == it->ip && ft_findKey(it->port, itP->ports) == SUCCESS)
					itP->ports.push_back(it->port);
		}
		else
		{
			newSock.ports.clear();
			newSock.ip = it->ip;
			newSock.ports.push_back(it->port);
			s_sock->push_back(newSock);
		}
	}
}

void	ft_initLocation(s_location &location)
{
	location.path = "";
	location.root = "";
	location.autoindex = 0;
	location.index = "";
	location.scriptName = "";
	location.extension = "";
}

void	ft_initServer(s_serverConfig &server)
{
	server.port = 0;
	server.ip = "";
	server.server_name = "";
	server.client_max_body_size = 1000000;
	server.root = "";
	server.default_server = 0;
}

void	ft_defaultLocation(std::vector<s_location> &locTab)
{
	s_location	defaultLoc;

	defaultLoc.path = "/";
	defaultLoc.root = "/";
	defaultLoc.index = "/";
	defaultLoc.methods.push_back("GET");
	locTab.push_back(defaultLoc);
}
