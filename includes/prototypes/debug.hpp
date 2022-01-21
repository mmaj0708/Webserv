/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 12:26:48 by jdussert          #+#    #+#             */
/*   Updated: 2021/12/06 22:48:04 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
# define DEBUG_HPP

# include "../common.hpp"

void	ft_print_serverTab(const std::vector<s_serverConfig> serverTab, const std::vector<short int> portTab, const std::vector<s_sock_setup> sockTab);

#endif
