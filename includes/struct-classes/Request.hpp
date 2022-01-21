/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 19:27:04 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/14 16:29:36 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

class Request
{
    private:

		short int		_statusCode;
	
		// From? To?
        short int		_clientSocket;
		short int		_serverSocket;
		std::string		_hostName;

		// Parsing
        std::string			_rawData; // remaining data to be parsed
		bool				_isChunked;
		bool				_isMultipart; // ajouter getter et surcharge op 
		std::string			_boundary; // ajouter getter et surcharge op
		unsigned long int	_contentLength;
        int					_parsingStatus; 
							// -1 = error 
							// 0 = didn't start
							// 1 = some headers data received
							// 2 = headers parsed
							// 3 = some body data received
							// 4 = done
	
        // Request Line
        std::string		_method;
        std::string		_URI;
		std::string		_query;
        std::string		_version;

        // Request Headers
        std::map<std::string, std::string>	_headers;

        // Request Body
        std::string		_body;

        

    public:
		Request();
        Request(short int clientSocket, short int serverSocket);
        Request(Request const &src);
        ~Request();

		Request	&operator=(Request const &rhs);

		// *** PARSING ***
		void	addRawData(char *buffer);
		// Headers (Request Line + Headers fields)
		bool	readyToParseHeaders();
		bool	parseHeaders();
		bool	parseRequestLine();
		// bool	cleanupURI();
		bool	checkRequestLine();
		bool	parseRequestHeaders();
		bool	checkRequestHeaders();
		bool	checkHostName();
		// Body
		bool	readyToParseBody();
		bool	parseBody();
		bool	checkBodyDetails();
		int		parseChunks();
		// Error
		bool	parsingError(short int code);
		
		// GETTERS
		short int		getStatusCode() const;
        short int		getClientSocket() const;
		short int		getServerSocket() const;
		std::string		getHostName() const;
        std::string		getRawData() const;
		bool			getIsChunked() const;
		unsigned long int	getContentLength() const;
        short int		getParsingStatus() const;
        std::string		getMethod() const;
        std::string		getURI() const;
		std::string		getQuery() const;
        std::string		getVersion() const;
        std::map<std::string, std::string>	getHeaders() const;
        std::string		getBody() const;
		std::string		getSpecificHeaderValue(std::string header);
		
       		
};

std::ostream	&operator<<(std::ostream &o, Request const &i);

#endif