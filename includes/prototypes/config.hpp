/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_config.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 12:26:12 by jdussert          #+#    #+#             */
/*   Updated: 2021/12/06 22:31:04 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_CONF_HPP
# define PARSING_CONF_HPP

# include "../common.hpp"

int							ft_findConfigParam(std::vector<std::string>::iterator *begin, s_serverConfig *newServer, const char *toFind, std::vector<s_serverConfig> &serverTab);
int							ft_findLocationParam(std::vector<std::string>::iterator begin, s_serverConfig *newServer, const char *toFind, std::vector<s_serverConfig> &serverTab);
s_location					ft_newLocation(std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, std::vector<s_serverConfig> &serverTab);
s_serverConfig				ft_newServer(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, bool default_server, std::vector<s_serverConfig> &serverTab);
int							ft_isInServer(std::vector<std::string> tab, std::vector<s_serverConfig> &serverTab);
std::vector<s_serverConfig>	ft_get_serverTab(std::vector<s_serverConfig> &serverTab, const char *file);
int							ft_check_file(std::string file);
int							ft_config_file(int ac, char **av, std::vector<s_serverConfig> *serverTab, std::vector<short int> *portTab, std::vector<s_sock_setup> *sockTab);

/*
**	SERVER BLOC FOUND
*/
void	ft_listenFound(std::vector<std::string>::iterator begin, s_serverConfig	*newServer, std::vector<s_serverConfig> &serverTab);
void	ft_rootFound(std::vector<std::string>::iterator begin, s_serverConfig *newServer, std::vector<s_serverConfig> &serverTab);
void	ft_serverNameFound(std::vector<std::string>::iterator begin, s_serverConfig *newServer, std::vector<s_serverConfig> &serverTab);
void	ft_errorPageFound(std::vector<std::string>::iterator begin, s_serverConfig *newServer, std::vector<s_serverConfig> &serverTab);
void    ft_indexFound(std::vector<std::string>::iterator begin, s_serverConfig *newServer, std::vector<s_serverConfig> &serverTab);
void	ft_maxBodyFound(std::vector<std::string>::iterator begin, s_serverConfig *newServer, std::vector<s_serverConfig> &serverTab);

/*
**	LOCATION BLOC FOUND
*/
void    ft_methodsFound(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab);
void    ft_locationRoot(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab);
void    ft_locationAutoindex(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab);
void    ft_locationFound(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab);
int		ft_locInBrackets(std::vector<std::string>::iterator *it, std::vector<std::string>::iterator *ite, std::vector<s_location> &locTab, std::vector<s_serverConfig> &serverTab);
int		ft_isInLocation(std::vector<std::string>::iterator *it, std::vector<std::string>::iterator ite, std::vector<s_location> &locTab, std::vector<s_serverConfig> &serverTab);
void    ft_locationIndex(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab);
void    ft_redirection(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab);

/*
**  CGI
*/
void		ft_parseCGI(std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab);

/*
**	UTILS
*/
void	    ft_locationPath(size_t *pos, std::vector<std::string>::iterator begin, s_location *location, std::vector<s_serverConfig> &serverTab);
void		ft_skip_isspace(std::vector<std::string>::iterator begin, size_t *pos, std::vector<s_serverConfig> &serverTab);
void		ft_check_bad_char(size_t idx, size_t end, std::vector<std::string>::iterator begin, std::string err, std::vector<s_serverConfig> &serverTab);
void		ft_cleanServerTab(std::vector<s_serverConfig> &serverTab);
void		ft_fill_portTab(std::vector<s_serverConfig> *serverTab, std::vector<short int> *portTab);
void		ft_initServer(s_serverConfig &server);
void		ft_initLocation(s_location &location);
int			ft_isInBrackets(std::vector<s_serverConfig> &serverTab, std::vector<std::string>::iterator *idx, std::vector<std::string>::iterator *tabEnd);
int			ft_isInServer(std::vector<std::string> tab, std::vector<s_serverConfig> &serverTab);
int			ft_error(std::vector<s_serverConfig> &serverTab, std::string err);
void		ft_check_spaces(std::vector<std::string>::iterator begin, size_t *pos, std::vector<s_serverConfig> &serverTab);
int			ft_findKey(short int key, std::vector<short int> haystack);
int			ft_findKey(std::string key, std::vector<s_sock_setup> s_sock);
void    	ft_fillSocketSetup(std::vector<s_serverConfig> const *serverTab, std::vector<s_sock_setup> *s_sock);
bool		ft_checkExtension(std::string str, std::string toFind);
void		ft_checkEndOfLine(std::vector<std::string>::iterator begin, size_t pos, std::vector<s_serverConfig> &serverTab, std::string param);
std::string	ft_firstWord(std::vector<std::string>::iterator begin, std::vector<s_serverConfig> &serverTab);
void	    ft_defaultLocation(std::vector<s_location> &locTab);

#endif
