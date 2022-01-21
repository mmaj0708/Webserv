/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 12:26:25 by jdussert          #+#    #+#             */
/*   Updated: 2021/12/21 21:44:24 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_HPP
# define GLOBAL_HPP

struct				s_global
{
	struct pollfd 					pollfds[MAX_CLIENTS + 1];
	nfds_t							nfds;
	int								nbVirtualHosts; // nombre de blocs serveurs
	int								nbServ;
	int								error; // 0 = good | -1 = error
	std::vector<Request>			pendingRequests;
};
static volatile int g_run = 1;

#endif
