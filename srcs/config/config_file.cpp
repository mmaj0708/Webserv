/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_file.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jehannedussert <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 13:50:42 by jehannedu         #+#    #+#             */
/*   Updated: 2022/01/15 13:51:00 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

int	ft_findConfigParam(std::vector<std::string>::iterator *begin, s_serverConfig *newServer, const char *toFind, std::vector<s_serverConfig> &serverTab)
{
	size_t found = (*begin)->find(toFind);

	if (found != std::string::npos)
	{
		if (!strcmp(toFind, "listen"))
			ft_listenFound(*begin, newServer, serverTab);
		else if (!strcmp(toFind, "root"))
			ft_rootFound(*begin, newServer, serverTab);
		else if (!strcmp(toFind, "server_name"))
			ft_serverNameFound(*begin, newServer, serverTab);
		else if (!strcmp(toFind, "error_page"))
			ft_errorPageFound(*begin, newServer, serverTab);
		else if (!strcmp(toFind, "client_max_body_size"))
			ft_maxBodyFound(*begin, newServer, serverTab);
		else
			return (FAILURE);
		return (SUCCESS);
	}
	return (FAILURE);
}

int	ft_findLocationParam(std::vector<std::string>::iterator *begin, s_location *location, const char *toFind, std::vector<s_serverConfig> &serverTab)
{
	size_t found = (*begin)->find(toFind);

	if (found != std::string::npos)
	{
		if (!strcmp(toFind, "location"))
			ft_locationFound(*begin, location, serverTab);
		else if (!strcmp(toFind, "return"))
			ft_redirection(*begin, location, serverTab);
		else if (!strcmp(toFind, "methods"))
			ft_methodsFound(*begin, location, serverTab);
		else if (!strcmp(toFind, "root"))
			ft_locationRoot(*begin, location, serverTab);
		else if (!strcmp(toFind, "autoindex"))
			ft_locationAutoindex(*begin, location, serverTab);
		else if (!strcmp(toFind, "index"))
			ft_locationIndex(*begin, location, serverTab);
		else if (!strcmp(toFind, "cgi"))
			ft_parseCGI(*begin, location, serverTab);
		else
			return (FAILURE);
		return (SUCCESS);
	}
	return (FAILURE);
}

s_location	ft_newLocation(std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, std::vector<s_serverConfig> &serverTab)
{
	s_location	location;
	std::string	toFind;
	ft_initLocation(location);

	for (; it != ite; it++)
		if (ft_findLocationParam(&it, &location, "location", serverTab) == SUCCESS)
			break;
	++it;
	for (; it != ite; it++)
	{
		toFind = ft_firstWord(it, serverTab);
		if (ft_findLocationParam(&it, &location, toFind.c_str(), serverTab) == FAILURE)
			ft_error(serverTab, "Configuration file: wrong keyword in the location block [" + toFind + "]");
	}
	if (!location.methods.size())
		location.methods.push_back("GET");
	return (location);
}

s_serverConfig	ft_newServer(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, bool default_server, std::vector<s_serverConfig> &serverTab)
{
	s_serverConfig			newServer;
	std::vector<s_location>	locTab;
	std::string				toFind = "";
	ft_initServer(newServer);

	if (default_server)
		newServer.default_server = 1;
	for (; begin != end; begin++)
	{
		toFind = ft_firstWord(begin, serverTab);
		if (!(*begin).empty() && !toFind.empty() && 
			ft_findConfigParam(&begin, &newServer, toFind.c_str(), serverTab) == FAILURE)
		{
			if (toFind == "location")
			{
				ft_isInLocation(&begin, end, locTab, serverTab);
				continue;
			}
			else
				ft_error(serverTab, "Configuration file: wrong keyword in the server block [" + toFind + "]");
		}
	}
	if (locTab.empty())
		ft_defaultLocation(locTab);
	newServer.location = locTab;
	if (!newServer.port)
		newServer.port = 80;
	if (newServer.ip.empty())
		newServer.ip = "0.0.0.0";
	return (newServer);
}

std::vector<s_serverConfig>	ft_get_serverTab(std::vector<s_serverConfig> &serverTab, const char *file)
{
	char						*line;
	int							fd = open(file, O_RDONLY);
	int							n;
	std::vector<std::string>	tab;

	while ((n = get_next_line(fd, &line)) == 1)
	{
		tab.push_back(line);
		free(line);
	}
	tab.push_back(line);
	free(line);
	close(fd);
	if (ft_isInServer(tab, serverTab) == FAILURE)
		ft_error(serverTab, "No server block.");

	return (serverTab);
}

int	ft_check_file(std::string file)
{
	std::ifstream				file_name(file);
	size_t						server_nb = 0;
	std::string					line;

	if (!file_name)
	{
		if (file_name.is_open())
			file_name.close();
		throw std::logic_error("Cannot open file.");
	}
	else if (!ft_checkExtension(file, "conf"))
	{
		file_name.close();
		throw std::logic_error("Wrong extension.");
	}
	while (getline(file_name, line))
		if (line.find("server") != std::string::npos)
			server_nb++;
	file_name.close();
	if (!server_nb)
		throw std::logic_error("Wrong configuration file.");

	return (SUCCESS);
}

int		ft_config_file(int ac, char **av, std::vector<s_serverConfig> *serverTab, std::vector<short int> *portTab, std::vector<s_sock_setup> *sockTab)
{
	if (ac == 2)
	{
		try
		{
			ft_check_file(av[1]);
			*serverTab = ft_get_serverTab(*serverTab, av[1]);
			ft_fill_portTab(serverTab, portTab);
			ft_fillSocketSetup(serverTab, sockTab);
			// ft_print_serverTab(*serverTab, *portTab, *sockTab);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return (FAILURE);
		}
	}
	else
	{
		std::cout << "Wrong number of arguments." << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}
