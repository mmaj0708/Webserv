/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 11:21:20 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/10 15:23:18 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../common.hpp"

class Response
{
	private:
		Response();
		
		int				_currentState; //-1 err - stop, 0 done, 1 in progress 
		short int		_statusCode; // 200, 400, 404, 405 etc
		// Request on which is based the Response
		Request			_req; // plutot chercher un numero de requete aue de copier comme un ane la requete

		// For?
        short int		_clientSocket;
		
		// Config
		s_serverConfig	_validVH;
		s_location		_validLoc;

		// Path
		std::string		_absolutePath;
		int				_pathType; //  0 == répertoire ,  1 == fichier , 2 == autres (osef), -1 == error (faire des masques binaire pour lisibilite)
		FILE			*_file;
		size_t			_fileSize;

		// Response data
		std::string		_contentToSend;
		std::string		_cgiResponse;

		// debug
		short int		_nCallSend; // nb de fois où la rep est allée ds send()		

		std::string		_redirect;
		
	public:
		Response(Request const req, s_serverConfig const VH);
		Response(Response const &src);
		~Response();

		void	matchLocation(); // trouver le bloc location
		bool	defineAbsolutePath();
		int		analyzeRequest();
		bool	checkMethods(); // verif que la methode est acceptée
		void	ft_cgi();
		void	ft_get();
		void	chunk(FILE *file);
		void	ft_post();
		void	ft_delete();
		bool	ft_send();
		void	ft_error();
		bool	ft_autoindex();

		bool	ft_redirection();
		
		void	buildHeader();

		short int	getStatusCode() const;
		short int	getClientSocket() const;
		int			getCurrentState() const;
		std::string	getContentToSend() const;
};

std::ostream	&operator<<(std::ostream &o, Response const &i);

#endif