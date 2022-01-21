/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responses.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 22:58:35 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/13 20:55:25 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSES_HPP
# define RESPONSES_HPP

/* ************************************************************************** */
/*							   RESPONSES_UTILS  	    					  */
/* ************************************************************************** */

size_t			match_size(std::string URI, std::string location);
int				detect_path_type(std::string path);
std::string		realpath_to_cpp_string(const char *path);
std::string		itoa_cpp(int nb);

/* ************************************************************************** */
/*							   	  ERROR_MSG		  	    					  */
/* ************************************************************************** */

std::string		generate_err_response(short int status_code, char *dt);
std::string		get_err_msg(short int status_code);

#endif
