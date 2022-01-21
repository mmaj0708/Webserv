/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 17:17:14 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/10 15:30:49 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include "../common.hpp"

class Cgi
{
	private:
		Cgi();
        char            **_args;
        char            **_env;
        Request         _req;
        s_location      _loc;
	    std::string     _getBuffFile;
	    std::fstream    _openArgfile;

	public:
		Cgi(Request req, s_location loc, s_serverConfig validVH, std::string pathFile);
		Cgi(Cgi const &src);
		~Cgi();

        std::string execCgi();
};

std::ostream	&operator<<(std::ostream &o, Cgi const &i);

#endif