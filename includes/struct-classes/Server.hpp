/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 18:46:28 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/14 14:28:49 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../common.hpp"

typedef	std::vector<s_serverConfig>		serverTab_type;

class Server
{
	private:
		int							_servSock; // fd du serveur qui ecoute sur _port
		int							_port;		// port sur lequel le serveur écoute
		std::string					_ip;		// IP du serveur
		serverTab_type				_virtualHosts; // liste des virtualHosts affectés à ce serveur
		bool						_endFlag; // quand le serv doit s arretr
	
	public:
		Server();
		Server(Server const &src);
		~Server();
		std::vector<Response>		_pendingResponses;
		std::vector<s_client>	clientSocks; // clients actuellement connectés sur le serveur (sock = fd)
		int					createSocket(const std::string ipString, const int port);
		int					addVirtualHosts(const serverTab_type serverConfigTab);
		int					addNewClient();
		int					readClientSocket(s_client client);
		void				generateResponse(Request req);
		bool				findExistingResponse(short int client_socket);
		int					writeClientSocket(short int client_socket);

		void				print_stats(int print_virtualHosts);

		int					getSocket() const;
		int					getPort() const;
		void				setEndFlag(bool status);
		bool				getEndFlag() const;
		serverTab_type		getVH() const;
};

#endif
