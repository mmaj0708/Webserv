/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 22:00:18 by vlugand-          #+#    #+#             */
/*   Updated: 2021/12/22 00:43:03 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

typedef	std::vector<s_serverConfig>		            serverTab_type;

int webserv(const serverTab_type serverTab, const std::vector<s_sock_setup> sockTab);
int     process_request(short int client_fd, Server & server);

/* ************************************************************************** */
/*							 		  POLL		  	    					  */
/* ************************************************************************** */

bool            checking_timeout(std::vector<Server> & server, size_t i);
void            rm_socket_from_pollfds(size_t i);
void	        pollhup(std::vector<Server> &servers, size_t i);
void	        pollerr(std::vector<Server> &servers, size_t i);
void	        pollin(std::vector<Server> &servers, size_t i);
void	        pollout(std::vector<Server> &servers, size_t i);
int				polling_clients_requests(std::vector<Server> &servers);
s_recipient  	find_recipient(int fd, std::vector<Server> &servers);

// debug
void            print_all_serv_stats(std::vector<Server> servers, int printVH);

#endif