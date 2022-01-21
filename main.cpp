/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 18:49:41 by jdussert          #+#    #+#             */
/*   Updated: 2022/01/17 12:04:20 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/common.hpp"

s_global	g_all;

int main(int ac, char **av)
{
	std::vector<s_serverConfig>		configServerTab;
	std::vector<short int>			portTab;
	std::vector<s_sock_setup> 		sockTab;
	int								ret;

	ret = ft_config_file(ac, av, &configServerTab, &portTab, &sockTab);
	if (ret == FAILURE)
		return (FAILURE);

    if (webserv(configServerTab, sockTab) == FAILURE)
		return FAILURE;	
	return (0);
}
