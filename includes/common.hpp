/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 12:26:40 by jdussert          #+#    #+#             */
/*   Updated: 2022/01/06 18:39:25 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_HPP
# define COMMON_HPP

/* ************************************************************************** */
/*									MACROS									  */
/* ************************************************************************** */

# define FAILURE -1
# define SUCCESS 0
# define SERVER_PORT 8080
# define BUFSIZE 4096
# define CGI_PIPE_BUFFER_SIZE 4096
// # define SENDBUF 4096
# define SENDBUF 4096
# define SERVER_BACKLOG 100
# define MAX_CLIENTS 300
# define TIMEOUT 30
# define TRUE	1
# define FALSE	0
# define ERROR  -1
# define SIZE 1024


/* ************************************************************************** */
/*								  LIBRARIES									  */
/* ************************************************************************** */

# include <string>
# include <cstring>
# include <string.h>
# include <iostream>
# include <fstream>
# include <sstream>
# include <arpa/inet.h>		// htons, htonl, ntohs, ntohl
# include <sys/select.h>	// select()
# include <sys/types.h>		// socket, accept, listen, bind
# include <sys/socket.h>	// socket, accept, listen, bind
# include <vector>
# include <fcntl.h>
# include <stdio.h>
# include <ctype.h>
# include <cstdlib>
# include <map>
# include <netdb.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <unistd.h>
# include <ctime>
# include <stdlib.h>
# include <poll.h>
# include <signal.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <climits>

/* ************************************************************************** */
/*								  HPP FILES									  */
/* ************************************************************************** */

# include "struct-classes/config_struct.hpp"
# include "prototypes/requests.hpp"
# include "struct-classes/Request.hpp"
# include "prototypes/responses.hpp"
# include "struct-classes/Response.hpp"
# include "struct-classes/Cgi.hpp"
# include "struct-classes/Server.hpp"
# include "struct-classes/recipient.hpp"
# include "struct-classes/global.hpp"
# include "../srcs/config/get_next_line/get_next_line.hpp"
# include "prototypes/config.hpp"
# include "prototypes/webserv.hpp"
# include "prototypes/debug.hpp"

#endif
