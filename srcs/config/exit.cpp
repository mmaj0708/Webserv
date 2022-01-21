/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jehannedussert <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 13:51:23 by jehannedu         #+#    #+#             */
/*   Updated: 2022/01/15 13:51:26 by jehannedu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

void	ft_cleanServerTab(std::vector<s_serverConfig> &serverTab)
{
	if (!serverTab.size())
		return ;
	for (std::vector<s_serverConfig>::iterator it = serverTab.begin(); it != serverTab.end(); it++)
	{
		if (it->location.size())
			it->location.clear();
		if (it->error_page.size())
			it->error_page.clear();
	}
	serverTab.clear();
}

int		ft_error(std::vector<s_serverConfig> &serverTab, std::string err)
{
	ft_cleanServerTab(serverTab);
	throw std::logic_error("[ERROR] " + err);
	return (FAILURE);
}
