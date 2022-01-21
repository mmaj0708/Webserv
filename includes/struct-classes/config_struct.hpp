/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_struct.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 12:26:25 by jdussert          #+#    #+#             */
/*   Updated: 2022/01/13 20:22:05 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_STRUCT_HPP
# define CONFIG_STRUCT_HPP

struct				s_sock_setup
{
	std::string				ip;
	std::vector<short int>	ports;
};

struct s_client
{
    int		socket;
    time_t  init_time;
    time_t  curr_time;
};

struct				s_location
{
	std::vector<std::string>			methods;
	std::string							path;
	std::map<short int, std::string>	redir;
	std::string							root;
	bool								autoindex;
	std::string							index;
	std::string							extension;
	std::string							scriptName;
};

struct				s_serverConfig
{
	std::vector<s_location>				location;
	short int							port;
	std::string							ip;
	std::string							server_name;
	std::map<short int, std::string>	error_page;
	size_t								client_max_body_size;
	std::string							root;
	int									default_server;
};

#endif
