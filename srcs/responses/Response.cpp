/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 11:23:30 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/14 15:35:59 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Response::Response()
{
	return ;
}

Response::Response(Request const req, s_serverConfig const VH)
{
	this->_currentState = 1;
	this->_absolutePath.clear();
	this->_pathType = -1;
	this->_file = NULL;
	this->_fileSize = 0;
	this->_contentToSend.clear();
	this->_cgiResponse.clear();
	this->_nCallSend = 0;
	this->_req = req;
	this->_validVH = VH;
	this->_clientSocket = this->_req.getClientSocket();
	this->_statusCode = this->_req.getStatusCode();
	this->matchLocation();
	if (this->_req.getBody().size() > this->_validVH.client_max_body_size)
		this->_statusCode = 413;
	if (this->_statusCode >= 400 || this->checkMethods() == FALSE)
	{
		this->ft_error();
		return ;
	}
	this->analyzeRequest();
	return ;
}

Response::Response(Response const &src)
{
	*this = src;
	return ;
}

Response::~Response()
{
	return ;
}

/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

void		Response::ft_error()
{
	time_t			now = time(0);
	char*			dt = ctime(&now);
	tm				*gmtm = gmtime(&now);
	std::string		path;

	dt = asctime(gmtm);
	for (std::map<short int, std::string>::iterator it = this->_validVH.error_page.begin(); it != this->_validVH.error_page.end(); ++it)
	{
		if (it->first == this->_statusCode)
		{
			path = this->_validVH.root + it->second;
			if (detect_path_type(path) != 1)
			{
				std::cerr << "error: wrong file path for error " << this->_statusCode << std::endl;
				break ;
			}
			if ((this->_file = fopen(path.c_str(), "r")) == NULL)
			{
				perror("ft_error: fopen");
				break ;
			}
			fseek(this->_file, 0, SEEK_END);
			this->_fileSize = ftell(this->_file);
			fseek(this->_file, 0, SEEK_SET);
			this->_contentToSend += "HTTP/1.1";
			this->_contentToSend += itoa_cpp(this->_statusCode);
			this->_contentToSend += "\r\n";
			this->_contentToSend += dt;
			this->_contentToSend += "Content-Length: ";
			this->_contentToSend += itoa_cpp(this->_fileSize);
			this->_contentToSend += "\r\n\r\n";
			this->_currentState = 1;
			return ;
		}
	}
	this->_contentToSend = generate_err_response(this->_statusCode, dt);
	this->_currentState = 1;
	return ;
}

bool		Response::ft_send()
{
	char	buffer[SENDBUF];
	ssize_t	ret;
	ssize_t	read_size;
	++(this->_nCallSend);

	if (!this->_contentToSend.empty())
	{
		ret = send(this->_clientSocket, this->_contentToSend.c_str(), this->_contentToSend.size(), 0);
		if (ret == -1 || ret == 0)
			return (FALSE);
		this->_contentToSend.clear();
		if (this->_contentToSend.empty() && !this->_file)
		{
			this->_currentState = 0;
			std::cout << "\033[1;32mEOF REACHED FOR SOCKET [" << this->getClientSocket() << "] AFTER [" << this->_nCallSend << "] SEND() | URI [" << _req.getURI() << "]\033[0m" << std::endl;
		}
		else
			this->_currentState = 1;
	}
	else if (this->_file)
	{
		bzero(buffer, SENDBUF);
		read_size = fread(buffer, 1, SENDBUF-1, this->_file);
		if (read_size == -1)
			return (FALSE);
		ret = send(this->getClientSocket(), buffer, read_size, 0);

		if (ret == -1)
			return (FALSE);
		if (feof(this->_file))
		{
			fclose(this->_file);
			this->_file = NULL;
			this->_currentState = 0;
			std::cout << "\033[1;32mEOF REACHED FOR SOCKET [" << this->getClientSocket() << "] AFTER [" << this->_nCallSend << "] SEND() | URI [" << _req.getURI() << "]\033[0m" << std::endl;
			return (TRUE);
		}
		this->_currentState = 1;
	}
	return (TRUE);
}

void	Response::matchLocation()
{
	size_t		best_match_index = 0;
	size_t		best_match_size = 0;
	size_t		current_match_size = 0;

	for (size_t i = 0; i < this->_validVH.location.size(); i++)
	{
		current_match_size = match_size(this->_req.getURI(), this->_validVH.location[i].path);
		if (current_match_size > best_match_size)
			best_match_index = i;
	}
	this->_validLoc = this->_validVH.location[best_match_index];
	return ;
}

bool	Response::ft_redirection()
{
	if (!this->_validLoc.redir.empty())
	{
		this->_statusCode = this->_validLoc.redir.begin()->first;
		this->_absolutePath = this->_validLoc.redir.begin()->second;
		this->_contentToSend += this->_req.getVersion();
		this->_contentToSend += " 301 Moved Permanently\r\n";
		this->_contentToSend += "Content-Length: 0\r\n";
		this->_contentToSend += "Location: http://";
		this->_contentToSend += this->_validVH.ip;
		this->_contentToSend += ":";
		this->_contentToSend += itoa_cpp(this->_validVH.port);
		this->_contentToSend += this->_absolutePath;
		this->_contentToSend += "\r\n\r\n";
		std::cout << this->_contentToSend << std::endl;
		return (TRUE);
	}
	return (FALSE);
}

bool	Response::defineAbsolutePath()
{
	std::string		tmp;
	size_t			match;

	if (!this->_absolutePath.empty())
		return SUCCESS;
	match = match_size(this->_req.getURI(), this->_validLoc.path);
	tmp = this->_req.getURI();	
	if (match)
		tmp.erase(0, match);
	if (this->_validLoc.root.empty())
		this->_absolutePath = this->_validVH.root + tmp;
	else
		this->_absolutePath = this->_validLoc.root + tmp;
	this->_pathType = detect_path_type(this->_absolutePath);
	if (this->_pathType == 0)
	{
		if (this->_validLoc.autoindex && this->_validLoc.index.empty())
		{
			this->ft_autoindex();
			return FALSE;
		}
		else 
		{
			this->_absolutePath += this->_validLoc.index;
			this->_pathType = detect_path_type(this->_absolutePath);
			if (this->_pathType != 1)
			{
				// std::cout << "ERROR: couldn't recreate path with index" << std::endl;
				this->_currentState = -1;
				this->_statusCode = 404;
				this->ft_error();
				return (FALSE);
			}
		}
	}
	else if (this->_pathType == -1 || this->_pathType == 2)
	{
		// std::cout << "ERROR: wrong path : " << this->_absolutePath << std::endl;
		this->_statusCode = 404;
		this->ft_error();
		return (FALSE);
	}
	this->_absolutePath = realpath_to_cpp_string(this->_absolutePath.c_str());
	return (TRUE);
}

bool	Response::checkMethods()
{
	for (size_t i = 0; i < this->_validLoc.methods.size(); i++)
	{
		if (this->_req.getMethod() != "GET" && this->_req.getMethod() != "POST" && this->_req.getMethod() != "DELETE")
		{
			this->_statusCode = 501;
			return (FALSE);
		}
		if (this->_req.getMethod() == this->_validLoc.methods[i])
			return (TRUE);
	}
	this->_statusCode = 405;
	return (FALSE);
}

int		Response::analyzeRequest()
{
	if (!this->_validLoc.scriptName.empty())
	{
		this->ft_cgi();
		return SUCCESS;
	}
	else if (this->ft_redirection())
		return (SUCCESS);
	if (!this->defineAbsolutePath())
		return (FAILURE);
    else if (_req.getMethod() == "GET")
        this->ft_get();
    else if (this->_req.getMethod() == "DELETE")
        this->ft_delete();
	return SUCCESS;
}

/* ************************************************************************** */
/*                               FT_AUTOINDEX                                 */
/* ************************************************************************** */

bool		Response::ft_autoindex()
{
	std::cout << "IN AUTOINDEX" << std::endl;
    DIR				*dp;
	struct dirent   *dirp;
	std::string		body;

 	dp = opendir(this->_absolutePath.c_str());
    if (dp == NULL)
    {
		this->_statusCode = 500;
		std::cout << "OPENDIR FAILED" << std::endl;
        return FAILURE;
    }
    body = "<html>\n<body>\n";
    body += "<h2>directory listing :</h2>\n";
	while ((dirp = readdir(dp)) != NULL)
	{
		body += "<a href=\"" + this->_req.getURI();
		if (this->_req.getURI()[this->_req.getURI().length() - 1] != '/')
			body += '/';
		body += dirp->d_name;
		body += "\">";
		body += dirp->d_name;
		body += "</a>";
        body += "<br>\n";
	}
    body += "</body>\n</html>\n";
	closedir(dp);

	this->_statusCode = 200;
	this->_contentToSend += "HTTP/1.1 200 OK\r\n";
	this->_contentToSend += "Content-Length: ";
	this->_contentToSend += itoa_cpp(body.size());
	this->_contentToSend += "\r\n";
	this->_contentToSend += "Content-type: text/html;\r\n";
	this->_contentToSend += "\r\n";
	this->_contentToSend += body;

	this->_currentState = 1;
	return 1;
}

/* ************************************************************************** */
/*                                  FT_CGI                                    */
/* ************************************************************************** */

void		Response::ft_cgi()
{
	if (!this->defineAbsolutePath())
		return ;
	Cgi		cgi(this->_req, this->_validLoc, this->_validVH, this->_absolutePath);
	this->_cgiResponse = cgi.execCgi();
	if (this->_cgiResponse.empty())
	{
		this->_statusCode = 500;
		this->ft_error();
		return ;
	}
	this->_cgiResponse.erase(this->_cgiResponse.begin(), this->_cgiResponse.begin() + this->_cgiResponse.find("\r\n\r\n") + 4);
	this->_statusCode = 200;
	this->_contentToSend += "HTTP/1.1 200 OK\r\n";
	this->_contentToSend += "Content-Length: ";
	this->_contentToSend += itoa_cpp(_cgiResponse.size());
	this->_contentToSend += "\r\n";
	this->_contentToSend += "X-Powered-By: PHP/8.0.2\r\n";
	this->_contentToSend += "Content-type: text/html; charset=UTF-8\r\n";
	this->_contentToSend += "\r\n";
	this->_contentToSend += _cgiResponse;
	return;
}

/* ************************************************************************** */
/*                                  FT_GET                                    */
/* ************************************************************************** */

void		Response::ft_get()
{
	if ((this->_file = fopen(this->_absolutePath.c_str(), "r")) == NULL)
	{
		perror("ft_get: fopen");
		this->_statusCode = 404;
		this->ft_error();
		return ;
	}
	fseek(this->_file, 0, SEEK_END);
	this->_fileSize = ftell(this->_file);
	fseek(this->_file, 0, SEEK_SET);
	this->_statusCode = 200;
	this->_contentToSend += "HTTP/1.1 200 OK\r\n";
	this->_contentToSend += "Content-Length: ";
	this->_contentToSend += itoa_cpp(this->_fileSize);
	this->_contentToSend += "\r\n\r\n";
	return ;
}

/* ************************************************************************** */
/*                                  FT_DELETE                                 */
/* ************************************************************************** */

void		Response::ft_delete()
{
	std::string		body;

	if ((this->_file = fopen(this->_absolutePath.c_str(), "r")) == NULL)
		perror("ft_delete: fopen");
	if (remove(this->_absolutePath.c_str()) != 0)
	{
		perror("ft_delete: cannot delete file");
		this->_statusCode = 400;
		this->ft_error();
		return ;
	}
	else
	{
		time_t	now = time(0);
		char*	dt = ctime(&now);
		tm		*gmtm = gmtime(&now);

		dt = asctime(gmtm);
		body = "<html>\n\t<body>\n\t\t<h1>File deleted.</h1>\n\t</body>\n</html>\n";
		this->_statusCode = 200;
		this->_contentToSend += "HTTP/1.1 200 OK\r\n";
		this->_contentToSend += "Date: ";
		this->_contentToSend += dt;
		this->_contentToSend += "Content-Length: ";
		this->_contentToSend += itoa_cpp(body.size());
		this->_contentToSend += "\r\n\r\n";
		this->_contentToSend += body;
	}
	fclose(this->_file);
	this->_file = NULL;
	return ;
}

/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */

short int	Response::getStatusCode() const
{
	return (this->_statusCode);
}

short int	Response::getClientSocket() const
{
	return (this->_clientSocket);
}

int			Response::getCurrentState() const
{
	return (this->_currentState);
}

std::string		Response::getContentToSend() const
{
	return (this->_contentToSend);
}

/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Response const &i)
{
	// std::cout << "\n/*************************** GENERAL INFO ***************************/\n" << std::endl;
	std::cout << "Status Code: " << '[' << i.getStatusCode() << ']' << std::endl;
	std::cout << "Client Socket: " << '[' << i.getClientSocket() << ']' << std::endl;
	std::cout << "Current State: " << '[' << i.getCurrentState() << ']' << std::endl;
	std::cout << "Content to send: \n" << '[' << i.getContentToSend() << ']' << std::endl;
	// std::cout << "\n/********************************************************************/" << std::endl;
    return (o);
}
