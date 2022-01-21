/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:17:07 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/14 13:38:59 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

#define PAGE	"<html>\n"\
             	"<head><title>ERROR</title></head>\n"\
               	"<body><center><h1>ERROR</h1></center>\n"\
				"<hr><center>Sorry :(</center></body>\n"\
				"</html>\n"

std::string		generate_err_response(short int status_code, char *dt)
{
	std::string response;
	std::string	err_msg;
	std::string	body = PAGE;
	size_t		pos;

	err_msg = get_err_msg(status_code);
	while ((pos = body.find("ERROR")) != std::string::npos)
	{
		body.erase(pos, 5);
		body.insert(pos, err_msg);
	}
	response = "HTTP/1.1 ";
	response += err_msg;
	response += "\r\nContent-Length: ";
	response += itoa_cpp(body.size());
	response += "\r\n";
	response += dt;
	response += "\r\n\r\n";
	response += body;
	return (response);
}

std::string		get_err_msg(short int status_code)
{
	switch (status_code)
	{
		case 400:
			return ("400 Bad Request");
        case 404:
            return ("404 Not Found");
        case 405:
            return ("405 Not Allowed");
        case 408:
            return ("408 Request Timeout");
        case 411:
            return ("411 Length Required");
        case 412:
            return ("412 Precondition Failed");
		case 413:
            return ("413 Payload Too Large");
        case 414:
            return ("414 URI Too Long");
		case 500:
            return ("500 Internal Server Error");
        case 501:
            return ("501 Not Implemented");
        case 505:
            return ("505 HTTP Version Not Supported");
	}
	return ("501 Not Implemented");
}