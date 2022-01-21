/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 21:13:20 by vlugand-          #+#    #+#             */
/*   Updated: 2021/12/08 14:58:26 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTS_HPP
# define REQUESTS_HPP

/* ************************************************************************** */
/*								REQUESTS_UTILS								  */
/* ************************************************************************** */

bool		URI_is_trespassing(std::string URI);
size_t		find_first_whitespace(const std::string s);
void		remove_first_ows(std::string & s);
void		str_toupper(std::string & s);
size_t		hex_str_to_int(std::string s);
size_t		find_existing_request(short int csocket);

#endif
