/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 23:21:43 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/14 15:19:52 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

extern	s_global g_all;

void	g_run_handler(int dummy)
{
	(void)dummy;
	g_run = 0;
}

void    print_all_serv_stats(std::vector<Server> servers, int printVH)
{
    for(size_t i = 0; i < servers.size(); i++)
        servers[i].print_stats(printVH);
}

void    ft_close(std::vector<Server> servers)
{
    for(size_t i = 0; i < servers.size(); i++)
        close(servers[i].getSocket());
    for(size_t i = servers.size(); i < MAX_CLIENTS; i++)
        if (g_all.pollfds[i].fd > 0)
            close(g_all.pollfds[i].fd);
}

void init_pollfds(std::vector<Server> servers)
{
    g_all.nfds = 0;
    for (size_t i = 0; i < MAX_CLIENTS + 1; i++)
    {
        g_all.pollfds[i].fd = 0;
    }
    for (size_t i = 0; i < servers.size(); i++)
    {
        g_all.pollfds[i].fd = servers[i].getSocket();
        g_all.pollfds[i].events = POLLIN;
        g_all.nfds++;
    }
}

std::vector<Server> setup_serv(const serverTab_type configServerTab, const std::vector<s_sock_setup> sockTab)
{
    std::vector<Server>     servers;
    std::string             ip;

    for (size_t i = 0; i < sockTab.size(); i++)
    {
        ip = sockTab[i].ip;
        for (size_t j = 0; j < sockTab[i].ports.size(); j++)
        {
            Server  newServer;
            if (newServer.createSocket(ip, sockTab[i].ports[j]) == FAILURE)
                g_all.error = FAILURE;
            newServer.addVirtualHosts(configServerTab);
			servers.push_back(newServer);
        }
    }
    return servers;
}

void	rm_outdated_requests(short int client_socket)
{
	size_t		i;

	for (i = 0 ; i < g_all.pendingRequests.size() ; i++)
	{
		if (client_socket == g_all.pendingRequests[i].getClientSocket())
			g_all.pendingRequests.erase(g_all.pendingRequests.begin() + i); 
	}
	return ;
}

int     process_request(short int client_socket, Server & server) 
{
	ssize_t		i;
	int			ret;

	if (!server.findExistingResponse(client_socket))
	{
		for (i = g_all.pendingRequests.size() - 1; i >= 0 ; i--)
		{
			if (client_socket == g_all.pendingRequests[i].getClientSocket())
			{
				server.generateResponse(g_all.pendingRequests[i]);
				rm_outdated_requests(client_socket);
				break ;
			}
		}
		if (i == -1)
			return (-1);
	}
	ret = server.writeClientSocket(client_socket);
	return (ret);
}

bool    checking_timeout(std::vector<Server> & server, size_t i)
{
    time_t  curr_time;
	s_recipient     r;

	r = find_recipient(g_all.pollfds[i].fd, server);
    if (r.servId == -1)
		return FALSE;
    time(&curr_time);
    if (difftime(curr_time, server[r.servId].clientSocks[r.cliId].init_time) > TIMEOUT)
        return (TRUE);
    return (FALSE);
}

int poll_loop(std::vector<Server> & servers)
{
    init_pollfds(servers);

    while (g_run)
    {
        int pollResult = poll(g_all.pollfds, MAX_CLIENTS, 2500);
        if (pollResult > 0)
        {
            for (size_t i = 0; i < servers.size(); i++)
            {
                if (g_all.pollfds[i].revents & POLLIN)
                {
                    if (!servers[i].addNewClient())
                    {
                        std::cout << "FATAL: could not add new client" << std::endl;
						return (FAILURE);
                    }
                    g_all.pollfds[i].revents = 0;
                    g_all.nfds++;
                }
            }
            polling_clients_requests(servers);
        }
        for (size_t i = servers.size(); i < g_all.nfds; i++)
            if (checking_timeout(servers, i))
			    pollhup(servers, i);
    }
    return 0;
}

int webserv(const serverTab_type configServerTab, const std::vector<s_sock_setup> sockTab)
{
	signal(SIGINT, g_run_handler);
	signal(SIGPIPE, SIG_IGN);

    std::vector<Server>     servers;
    int ret;
    g_all.error = 0;    
    servers = setup_serv(configServerTab, sockTab);
    if (g_all.error == FAILURE)
    {
        ft_close(servers);
        return FAILURE;
    }
    ret = poll_loop(servers);
    ft_close(servers);

    std::cout << "\033[1;37mServer End\033[0m" << std::endl;

    return ret;
}
