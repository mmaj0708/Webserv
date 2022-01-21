/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 20:16:27 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/14 18:10:31 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

extern	s_global g_all;

/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Server::Server()
{
	this->_servSock = 0;
	this->_endFlag = 0;
	return ;
}

Server::Server(Server const &src)
{
	*this = src;
	return ;
}

Server::~Server()
{
	return ;
}

/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

int	Server::addVirtualHosts(const serverTab_type serverConfigTab)
{
	for (size_t i = 0; i < serverConfigTab.size(); i++)
	{
		if (serverConfigTab[i].ip == _ip && serverConfigTab[i].port == _port)
			_virtualHosts.push_back(serverConfigTab[i]);
	}
	return 0;
}

int Server::createSocket(const std::string ip, const int port)
{
	u_int32_t   binaryIp = 0;
    int on = true;
    struct sockaddr_in addr;


	inet_pton(AF_INET, ip.c_str(), &binaryIp);
    if ((this->_servSock = socket(AF_INET, SOCK_STREAM, 0)) == FAILURE)
	{
		perror("socket() failed");
		return (FAILURE);
	}
	this->_port = port;
	this->_ip = ip;

    if (setsockopt(this->_servSock, SOL_SOCKET,  SO_REUSEADDR, &on, sizeof(on)) == FAILURE)
	{
		perror("setsockopt() failed");
		return (FAILURE);
	}
	if (fcntl(this->_servSock, F_SETFL, O_NONBLOCK) == FAILURE)
	{
		perror("fcntl() failed");
		return (FAILURE);
	}
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(binaryIp);

    if (bind(this->_servSock, (struct sockaddr *)&addr, sizeof(addr)) == FAILURE)
    {
        perror("bindResult");
		return (FAILURE);
    }
    if (listen(this->_servSock, SERVER_BACKLOG) == FAILURE)
    {
        perror("listenResult");
		return (FAILURE);
    }
    std::cout << "\033[1;37m\n** server start [" << port << "] **\n\033[0m" << std::endl;
    return this->_servSock;
}

int Server::addNewClient()
{
    struct sockaddr_in cliaddr;
	s_client			client;
	int on = true;
    int addrlen = sizeof(cliaddr);
    int client_socket = accept(this->_servSock, (struct sockaddr *)&cliaddr, (socklen_t*)&addrlen);
	
	if (client_socket < 0)
	{
		perror("accept failure:");
		return (-1);
	}
	client.socket = client_socket;
	time(&client.init_time);
	if (client.socket == -1)
	{
		perror("accept() failed");
		return (FAILURE);
	}
    if (setsockopt(client.socket, SOL_SOCKET,  SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		perror("setsockopt() failed");
		return (0);
	}
	if (fcntl(client.socket, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl() failed");
		return (0);
	}

	std::cout << "\033[1;35mADDING CLIENT_SOCKET [" << client_socket << "] ON SERVER ::" << this->_port << "\033[0m" << std::endl;
	this->clientSocks.push_back(client);
	for (int i = g_all.nbServ; i < MAX_CLIENTS; i++)
    {
        if (g_all.pollfds[i].fd == 0)
        {
			std::cout << "\033[1;35mADDING CLIENT_SOCKET [" << client_socket << "] AT POLLFD[" << i << "] ON SERVER ::" << this->_port << "\033[0m" << std::endl;
            g_all.pollfds[i].fd = client_socket;
            g_all.pollfds[i].events = POLLIN | POLLHUP | POLLERR;
            break ;
        }
    }
    return (1);
}

int		Server::readClientSocket(s_client client)
{
	int			ret;
	char		buffer[BUFSIZE + 1];
	size_t		i;

	i = find_existing_request(client.socket);
	if (i == g_all.pendingRequests.size())
		g_all.pendingRequests.push_back(Request(client.socket, this->getSocket()));
	ret = recv(client.socket, buffer, BUFSIZE, 0);
	time(&client.init_time);
	if (ret == 0)
		return (-1);
	if (ret == -1)
	{
		perror("recv failure:");
		return (-1);
	}
	if ((i = find_existing_request(client.socket)) == g_all.pendingRequests.size())
		return (-1);
	buffer[ret] = '\0';
	g_all.pendingRequests[i].addRawData(buffer);
	if (g_all.pendingRequests[i].readyToParseHeaders() && !g_all.pendingRequests[i].parseHeaders())
		return (0);
	else if (g_all.pendingRequests[i].readyToParseBody() && !g_all.pendingRequests[i].parseBody())
		return (0);
	if (g_all.pendingRequests[i].getParsingStatus() == 4)
		return (0);
	return (1);
} 


int		Server::writeClientSocket(short int client_socket)
{
	size_t		i;
	int			ret;

	for (i = 0; i < this->_pendingResponses.size(); i++)
	{
		if (client_socket == this->_pendingResponses[i].getClientSocket())
			break ;
	}
	if (i == this->_pendingResponses.size())
		return (-1);
	if (this->_pendingResponses[i].getCurrentState() == -1)
		return (-1);
	ret = this->_pendingResponses[i].ft_send();
	if (ret == FALSE)
		return (-1);
	if (this->_pendingResponses[i].getCurrentState() == 0)
	{
		this->_pendingResponses.erase(this->_pendingResponses.begin() + i);
		return (0);
	}
	return (1);
}

void	Server::generateResponse(Request req)
{
	size_t		i;
	std::cout << "\033[1;36m>>> GENERATE RESPONSE FOR [" << req.getURI() << "]" << " on sock [" << req.getClientSocket() << "]\033[0m" << std::endl;
	for (i = 0; i < this->_virtualHosts.size(); i++)
	{
		if (this->_virtualHosts[i].server_name == req.getHostName())
			break ;
	}
	if (i == this->_virtualHosts.size())
		i = 0;
	this->_pendingResponses.push_back(Response(req, this->_virtualHosts[i]));
	return ;	
}

bool	Server::findExistingResponse(short int client_socket)
{
	size_t		i;

	for (i = 0; i < this->_pendingResponses.size(); i++)
	{
		if (client_socket == this->_pendingResponses[i].getClientSocket())
			return (TRUE);
	}
	return (FALSE);
}


/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */

int				Server::getSocket() const
{
	return (this->_servSock);
}

int				Server::getPort() const
{
	return (this->_port);
}

bool			Server::getEndFlag() const
{
	return (this->_endFlag);
}

void			Server::setEndFlag(bool status)
{
	this->_endFlag = status;
	return ;
}

serverTab_type	Server::getVH()	const
{
	return (this->_virtualHosts);
}

/* ************************************************************************** */
/*                             		DEBUG		                              */
/* ************************************************************************** */

void				Server::print_stats(int print_virtualHost)
{
	std::cout << "\nSERVER STATS" << std::endl;
	std::cout << "IP/PORT		 : " << _ip << ":" << _port << std::endl;
	std::cout << "Serveur_Socket : " << _servSock << std::endl;
	std::cout << "Client_Sockets : ";
	for(size_t i = 0; i < clientSocks.size(); i++)
		std::cout << clientSocks[i].socket << " ";
	std::cout << std::endl;
	std::cout << "NbVirtualHosts : " << _virtualHosts.size() << "\n" << std::endl;
	if (print_virtualHost)
	{
		std::vector<s_serverConfig>::const_iterator it = _virtualHosts.begin();
		std::vector<s_serverConfig>::const_iterator	ite = _virtualHosts.end();
		int i = 1;

		for (; it != ite; it++)
		{
			int j = 1;
			std::cout << "\n";
			std::cout << "############### VIRTUALHOSTS [" << i << "] ###############\n\n";
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
	}
}

