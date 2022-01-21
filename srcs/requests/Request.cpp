/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 19:33:12 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/14 18:20:00 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

/* ************************************************************************** */
/*                        CONSTRUCTORS / DESTRUCTORS                          */
/* ************************************************************************** */

Request::Request()
{
    return ;
}

Request::Request(short int clientSocket, short int serverSocket)
{
	this->_serverSocket = serverSocket;
    this->_clientSocket = clientSocket;
	this->_statusCode = 0;
	this->_parsingStatus = 0;
	this->_isChunked = 0;
	this->_isMultipart = 0;
	this->_hostName.clear();
	this->_rawData.clear();
	this->_contentLength = 0;
	this->_method.clear();
	this->_URI.clear();
	this->_query.clear();
	this->_version.clear();
	this->_headers.clear();
	this->_body.clear();
    return ;
}

Request::Request(Request const &src)
{
    *this = src;
    return ;
}

Request::~Request()
{
    return ;
}

/* ************************************************************************** */
/*                            OPERATORS OVERLOAD                              */
/* ************************************************************************** */

Request		&Request::operator=(Request const &rhs)
{
	this->_statusCode = rhs.getStatusCode();
	this->_clientSocket = rhs.getClientSocket();
	this->_serverSocket = rhs.getServerSocket();
	this->_hostName = rhs.getHostName();
	this->_rawData = rhs.getRawData();
	this->_isChunked = rhs.getIsChunked();
	this->_contentLength = rhs.getContentLength();
	this->_parsingStatus = rhs.getParsingStatus(); 
	this->_method = rhs.getMethod();
	this->_URI = rhs.getURI();
	this->_query = rhs.getQuery();
	this->_version = rhs.getVersion();
	this->_headers = rhs.getHeaders();
	this->_body = rhs.getBody();
	return (*this);
}

/* ************************************************************************** */
/*                             MEMBER FUNCTIONS                               */
/* ************************************************************************** */

void		Request::addRawData(char *buffer)
{
	this->_rawData += buffer;
	if (this->_parsingStatus == 0)
	{
		while (this->_rawData.size() >= 2 && this->_rawData[0] == '\r' && this->_rawData[1] == '\n')
			this->_rawData.erase(0, 2);
		this->_parsingStatus = 1;
	}
	return ;
}

bool	Request::readyToParseHeaders()
{
	if ((this->_parsingStatus == 0 || this->_parsingStatus == 1) && this->_rawData.find("\r\n\r\n") != std::string::npos)
		return (TRUE);
	return (FALSE);
}

bool	Request::checkRequestLine()
{
	for (std::string::iterator it = this->_method.begin() ; it != this->_method.end() ; ++it)
	{
        if (*it < 'A' || *it > 'Z')
			return (this->parsingError(400));
	}
	if (URI_is_trespassing(this->_URI))
		return (this->parsingError(400));
	if (this->_URI.size() > 1024)
		return (this->parsingError(414));
	if (this->_version != "HTTP/1.1" && this->_version != "HTTP/1.0")
		return (this->parsingError(505));
	return (TRUE);
}

bool	Request::parseRequestLine()
{
	std::string	value;
	size_t		pos;
	int			i;

	for (i = 0; i < 3; i++)
	{
		if (i == 0 || i == 1)
			pos = this->_rawData.find(' ');
		else
			pos = this->_rawData.find("\r\n");
		value = this->_rawData.substr(0, pos);
		this->_rawData.erase(0, pos + 1);
		if (i == 0)
			this->_method = value;
		else if (i == 1)
			this->_URI = value;
		else if (i == 2)
			this->_version = value;
	}
	this->_rawData.erase(0, 1);
	if (!checkRequestLine())
		return (FALSE);
	return (TRUE);
}

bool	Request::checkRequestHeaders()
{
	for (std::string::iterator it = this->_rawData.begin(); it != this->_rawData.end(); it++)
	{
		if (*it == ':')
			return (TRUE);
		if (*it == ' ' || (*it >= 9 && *it <= 13))
			return (this->parsingError(400));
	}
	return (TRUE);
}

bool	Request::parseRequestHeaders()
{
	std::string	key;
	std::string	value;
	size_t		pos;
	std::pair<std::map<std::string,std::string>::iterator,bool> ret;

	while (this->_rawData.find("\r\n") != 0)
	{
		if (!checkRequestHeaders())
			return (FALSE);
		pos = this->_rawData.find(':');
		key = this->_rawData.substr(0, pos);
		str_toupper(key);
		this->_rawData.erase(0, pos + 1);
		remove_first_ows(this->_rawData);
		pos = this->_rawData.find("\r\n");
		value = this->_rawData.substr(0, pos);
		this->_rawData.erase(0, pos + 2);
		ret = this->_headers.insert(std::pair<std::string, std::string>(key, value));
		if (ret.second == FALSE && (key == "HOST" || key == "CONTENT-LENGHT"))
			return (this->parsingError(400));
	}
	this->_rawData.erase(0, 2);
	return (TRUE);
}

bool	Request::checkHostName()
{
	if (this->_headers.find("HOST") != this->_headers.end())
		this->_hostName = this->_headers.find("HOST")->second;
	else
	{
		this->_hostName = "UNSET";
		return (this->parsingError(400));
	}
	return (TRUE);
}

bool	Request::checkBodyDetails()
{
	std::string tmp = "UNSET";
	size_t		i;
	
	for(std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
	{
		if (it->first == "CONTENT-TYPE" && it->second.find("multipart/form-data") != std::string::npos)
		{
			this->_isMultipart = TRUE;
			this->_boundary = it->second;
			i = this->_boundary.find_first_of('=');
			this->_boundary.erase(0, i + 1);
			break ;
		}
	}
	if (this->_headers.find("TRANSFER-ENCODING") != this->_headers.end())
	{
		tmp = this->_headers.find("TRANSFER-ENCODING")->second;
		if (tmp == "chunked")
			this->_isChunked = TRUE;
		return (TRUE);
	}
	if (this->_headers.find("CONTENT-LENGTH") != this->_headers.end())
		tmp = this->_headers.find("CONTENT-LENGTH")->second;
	if (tmp == "UNSET")
		return (this->parsingError(411));
	if (tmp != "UNSET" && tmp.find_first_not_of("0123456789") != std::string::npos)
		return (this->parsingError(400));
	this->_contentLength = atoi(tmp.c_str());
	if (this->_contentLength < 0)
		return (this->parsingError(400));
	return (TRUE);
}

int		Request::parseChunks()
{
	size_t				pos;
	ssize_t				len;
	std::string			tmp;
	static ssize_t		chunk_size = -1;

		if (chunk_size == -1 && !this->_rawData.empty())
		{
			pos = this->_rawData.find("\r\n");
			tmp = this->_rawData.substr(0, pos);
			chunk_size = hex_str_to_int(tmp);
			this->_rawData.erase(0, pos + 2);
		}
		if (chunk_size == 0 && !this->_rawData.empty() && this->_rawData == "\r\n")
		{	
			chunk_size = -1;
			return (0);
		}
		else if (chunk_size > 0 && !this->_rawData.empty())
		{
			pos = this->_rawData.find("\r\n");
			tmp = this->_rawData.substr(0, pos);
			len = tmp.length();
			if (chunk_size != len)
			{
				chunk_size = -1;
				return (-1);
			}
			chunk_size = -1;
			this->_body += tmp;
			this->_rawData.erase(0, pos + 2);
		}
	return (1);
}

bool	Request::parseBody()
{
	int ret;
	this->checkBodyDetails();
	if (this->_isMultipart && this->_rawData.find(this->_boundary))
	{
		this->_body += this->_rawData;
		this->_rawData.clear();
		if (this->_body.find(this->_boundary + "--") != std::string::npos)
			this->_parsingStatus = 4;
	}
	else if (!this->_isChunked) 
	{
		if (this->_rawData.length() != this->_contentLength)
			return (this->parsingError(400));
		this->_body = this->_rawData;
		this->_rawData.clear();
		this->_parsingStatus = 4;
	}
	else
	{
		std::cout << "CHUNKED BODY RECEIVED" << std::endl;
		ret = this->parseChunks();
		if (ret == -1)
			return (this->parsingError(400));
		else if (ret == 0)
		{
			this->_parsingStatus = 4;
		}
		else
			this->_parsingStatus = 3;
	}
	return (TRUE);
}

bool	Request::parseHeaders()
{
	if (!parseRequestLine() || !parseRequestHeaders())
		return (FALSE);
	if (!checkHostName())
		return (FALSE);
	if (this->_headers.find("CONTENT-LENGTH") == this->_headers.end() && this->_headers.find("TRANSFER-ENCODING") == this->_headers.end())
		this->_parsingStatus = 4;
	else
		this->_parsingStatus = 2;
	return (TRUE);
}

bool	Request::readyToParseBody()
{
	if (this->_parsingStatus == 2 || this->_parsingStatus == 3)
		return (TRUE);
	return (FALSE); 
}

bool	Request::parsingError(short int code)
{
	this->_statusCode = code;
	this->_parsingStatus = 4;
	return (FALSE);
}

/* ************************************************************************** */
/*                             GETTERS / SETTERS                              */
/* ************************************************************************** */

short int		Request::getStatusCode() const
{
	return (this->_statusCode);
}

short int		Request::getClientSocket() const
{
	return (this->_clientSocket);
}

short int		Request::getServerSocket() const
{
	return (this->_serverSocket);
}

std::string		Request::getHostName() const
{
	return (this->_hostName);
}

std::string		Request::getRawData() const
{
	return (this->_rawData);
}

bool			Request::getIsChunked() const
{
	return (this->_isChunked);
}

unsigned long int	Request::getContentLength() const
{
	return (this->_contentLength);
}

short int		Request::getParsingStatus() const
{
	return (this->_parsingStatus);
}

std::string		Request::getMethod() const
{
	return (this->_method);
}

std::string		Request::getURI() const
{
	return (this->_URI);
}

std::string		Request::getQuery() const
{
	return (this->_query);
}

std::string		Request::getVersion() const
{
	return (this->_version);
}

std::map<std::string, std::string>	Request::getHeaders() const
{
	return (this->_headers);
}

std::string		Request::getBody() const
{
	return (this->_body);
}

std::string		Request::getSpecificHeaderValue(std::string header)
{
	for(std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
	{
		if (it->first == header)
			return (it->second);
	}
	return ("");
}

/* ************************************************************************** */
/*                            NON MEMBER OVERLOAD                             */
/* ************************************************************************** */

std::ostream	&operator<<(std::ostream &o, Request const &i)
{
	// std::cout << "\n/*************************** GENERAL INFO ***************************/\n" << std::endl;
	std::cout << "Status Code: " << '[' << i.getStatusCode() << ']' << std::endl;
	std::cout << "Client Socket: " << '[' << i.getClientSocket() << ']' << std::endl;
	std::cout << "Parsing Status: " << '[' << i.getParsingStatus() << ']' << std::endl;
	std::cout << "Raw Data remaining (to be parsed): \n" << '[' << i.getRawData() << ']' << std::endl;
	// std::cout << "\n/********************************************************************/" << std::endl;

	// std::cout << "\n/***************************** REQUEST *****************************/\n" << std::endl;
	// std::cout << "### REQUEST LINE ###" << std::endl;
	std::cout << "Method: " << '[' << i.getMethod() << ']' << std::endl;
	std::cout << "URI: " << '[' << i.getURI() << ']' << std::endl;
	std::cout << "Query: " << '[' << i.getQuery() << ']' << std::endl;
	// std::cout << "Version: " << '[' << i.getVersion() << ']' << std::endl;

	std::cout << "\n### REQUEST HEADERS ###" << std::endl;
	std::cout << "std::Map begin ---------------------------------------------------------" << std::endl;
	std::map<std::string, std::string> map = i.getHeaders();
	for(std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); ++it)
		std::cout << '[' << it->first << "] [" << it->second << "]\n";
	std::cout << "--------------------------------------------------------------------- End" << std::endl;

	std::cout << "\n### REQUEST BODY ###" << std::endl;
	std::cout << "[" << i.getBody() << "]" << std::endl;

	// std::cout << "\n/************************** END OF REQUEST **************************/" << std::endl;
    return (o);
}
