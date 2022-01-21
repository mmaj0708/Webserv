/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 19:28:33 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/13 19:08:06 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

extern	s_global g_all;

void	rm_socket_from_pollfds(size_t i)
{
	close(g_all.pollfds[i].fd);
	for (; i < g_all.nfds - 1; i++)
	{
		g_all.pollfds[i].fd = g_all.pollfds[i + 1].fd;
		g_all.pollfds[i].events = g_all.pollfds[i + 1].events;
		g_all.pollfds[i].revents =  g_all.pollfds[i + 1].revents;
	}
	g_all.pollfds[i].fd = 0;
	g_all.pollfds[i].events = 0;
	g_all.pollfds[i].revents =  0;
	g_all.nfds--;
	return ;
}

void	pollhup(std::vector<Server> &servers, size_t i)
{
	s_recipient     r;

	std::cout << "\n\033[1;31m>>> POLLHUP CALLED ON SOCKET : " << g_all.pollfds[i].fd << " <<<\033[0m" << std::endl;
	r = find_recipient(g_all.pollfds[i].fd, servers);
	for (size_t i = 0; i < servers[r.servId]._pendingResponses.size(); i++)
		if (g_all.pollfds[i].fd == servers[r.servId]._pendingResponses[i].getClientSocket())
			servers[r.servId]._pendingResponses.erase(servers[r.servId]._pendingResponses.begin() + i);
	for (size_t i = 0; i < g_all.pendingRequests.size(); i++)
	{
		if (g_all.pollfds[i].fd == g_all.pendingRequests[i].getClientSocket())
			g_all.pendingRequests.erase(g_all.pendingRequests.begin() + i);
	}
	servers[r.servId].clientSocks.erase(servers[r.servId].clientSocks.begin() + r.cliId);
	rm_socket_from_pollfds(i);
	return ;
}

void	pollerr(std::vector<Server> &servers, size_t i)
{
	s_recipient     r;

	std::cout << "\n>>> POLLERR CALLED <<<" << std::endl;
	r = find_recipient(g_all.pollfds[i].fd, servers);
	servers[r.servId].clientSocks.erase(servers[r.servId].clientSocks.begin() + r.cliId);
	rm_socket_from_pollfds(i);
	return ;
}

void	pollin(std::vector<Server> &servers, size_t i)
{
	s_recipient 	r;
	int				ret;

	r = find_recipient(g_all.pollfds[i].fd, servers);
	ret = servers[r.servId].readClientSocket(servers[r.servId].clientSocks[r.cliId]);
	g_all.pollfds[i].revents = 0;
	if (ret == -1)
	{
		std::cerr << "\033[1;31mERROR IN READING CLIENT SOCKET: removing client\033[0m" << std::endl;
		servers[r.servId].clientSocks.erase(servers[r.servId].clientSocks.begin() + r.cliId);
		rm_socket_from_pollfds(i);
	}
	else if (ret == 0)
		g_all.pollfds[i].events = POLLOUT | POLLHUP | POLLERR;
	return ;
}

void	pollout(std::vector<Server> &servers, size_t i)
{
	s_recipient     r;
	int				ret;

	g_all.pollfds[i].revents = 0;
	r = find_recipient(g_all.pollfds[i].fd, servers);
	ret = process_request(g_all.pollfds[i].fd, servers[r.servId]);
	if (ret == -1)
	{
		std::cerr << "\033[1;31mERROR IN PROCESSING REQUEST: removing client\033[0m" << std::endl;
		servers[r.servId].clientSocks.erase(servers[r.servId].clientSocks.begin()+r.cliId);
		rm_socket_from_pollfds(i);
	}
	else if (ret == 0)
		g_all.pollfds[i].events = POLLIN | POLLHUP | POLLERR;
	return ;
}

int		polling_clients_requests(std::vector<Server> &servers)
{
    for (size_t i = servers.size(); i < g_all.nfds; i++)
    {
		if (g_all.pollfds[i].fd > 0 && (g_all.pollfds[i].revents & POLLHUP) == POLLHUP)
			pollhup(servers, i);
		else if (g_all.pollfds[i].fd > 0 && (g_all.pollfds[i].revents & POLLERR) == POLLERR)
			pollerr(servers, i);
		else if (g_all.pollfds[i].fd > 0 && (g_all.pollfds[i].revents & POLLOUT) == POLLOUT)
			pollout(servers, i);
        else if (g_all.pollfds[i].fd > 0 && (g_all.pollfds[i].revents & POLLIN) == POLLIN)
			pollin(servers, i);
    }
    return 0;
}

s_recipient  find_recipient(int fd, std::vector<Server> &servers)
{
	s_recipient     recipient;

    for (size_t k = 0; k < servers.size(); k++)
    {
        for (size_t l = 0; l < servers[k].clientSocks.size(); l++)
		{
            if (!servers[k].clientSocks.empty() && fd == servers[k].clientSocks[l].socket)
			{
				recipient.servId = k;
				recipient.cliId = l;
				return recipient;
			}
		}	
    }
	recipient.servId = -1;
	recipient.cliId = -1;
	return recipient;
}
