/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug_config.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jehannedussert <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 13:54:07 by jehannedu         #+#    #+#             */
/*   Updated: 2022/01/15 13:54:09 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

void    ft_print_serverTab(const std::vector<s_serverConfig> serverTab, const std::vector<short int> portTab, const std::vector<s_sock_setup> sockTab)
{
	std::vector<s_serverConfig>::const_iterator it = serverTab.begin();
	std::vector<s_serverConfig>::const_iterator	ite = serverTab.end();
    int i = 1;

	for (; it != ite; it++)
	{
		int j = 1;
		std::cout << "\n";
        std::cout << "############### SERVER [" << i << "] ###############\n\n";
        std::cout << "port: " << it->port << std::endl;
        std::cout << "IP: " << it->ip << std::endl;
        std::cout << "server_name: " << it->server_name << std::endl;
        std::cout << "max_body_size: " << it->client_max_body_size << std::endl;
        std::cout << "root: " << it->root << std::endl;
        std::cout << "default_server: " << it->default_server << std::endl;
		for (std::map<short int, std::string>::const_iterator err_it = it->error_page.begin(); err_it != it->error_page.end(); err_it++)
		{
			std::cout << "\n*** ERROR PAGE [" << j << "] ***\n";
			std::cout << "error_pages code: " << err_it->first << std::endl;
			std::cout << "error_pages path: " << err_it->second << std::endl;
			std::cout << "\n";
			j++;
		}
		j = 1;
        for (std::vector<s_location>::const_iterator loc_it = it->location.begin(); loc_it != it->location.end(); loc_it++)
		{
        	std::cout << "\n*** LOCATION [" << j << "] ***\n";
			std::cout << "path: " << loc_it->path << std::endl;
			std::cout << "root: " << loc_it->root << std::endl;
			std::cout << "autoindex: " << loc_it->autoindex << std::endl;
			std::cout << "index: " << loc_it->index << std::endl;
			std::cout << "redirection:\n";
			for (std::map<short int, std::string>::const_iterator redir = loc_it->redir.begin(); redir != loc_it->redir.end(); redir++)
			{
				std::cout << "redirection code: " << redir->first << std::endl;
				std::cout << "redirection path: " << redir->second << std::endl;
				std::cout << "\n";
			}
			std::cout << "methods: ";
			for (std::vector<std::string>::const_iterator it_meth = loc_it->methods.begin(); it_meth != loc_it->methods.end(); it_meth++)
				std::cout << *it_meth << ' ';
			std::cout << "\n";
			std::cout << "CGI extension: " << loc_it->extension << std::endl;
			std::cout << "CGI script name: " << loc_it->scriptName << std::endl;
			j++;
		}
		std::cout << "\n";
        i++;
	}

	(void)portTab;
	i = 1;
	std::cout << "\n";
    for (std::vector<s_sock_setup>::const_iterator itTab = sockTab.begin(); itTab != sockTab.end(); itTab++)
	{
		std::cout << "\n";
        std::cout << "############### IP AND PORTS ###############\n\n";
        std::cout << "IP: " << itTab->ip << std::endl;
		for (std::vector<short int>::const_iterator itPort = itTab->ports.begin(); itPort != itTab->ports.end(); itPort++)
			std::cout << "port: " << *itPort << std::endl;
		std::cout << "\n";

        i++;
	}

    return ;
}
