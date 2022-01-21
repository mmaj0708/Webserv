/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responses_utils.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlugand- <vlugand-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 23:54:10 by vlugand-          #+#    #+#             */
/*   Updated: 2022/01/13 20:22:29 by vlugand-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.hpp"

size_t	match_size(std::string URI, std::string location) // fonction non membre
{
	size_t	i = 0;
	std::string::iterator it;

	if (location.size() > URI.size())
		return (0);
	for (it = URI.begin(); it != URI.end(); it++)
	{
		if (*it != location[i])
			break ;
		i++;
	}
	if (location[i])
		return (0);
	if (*it != '/' && it != URI.end())
		return (0);
	return (i);
}

int		detect_path_type(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == -1)
	{
		return (-1);
	}
	if(s.st_mode & S_IFDIR)
		return (0);
	else if(s.st_mode & S_IFREG)
		return (1);
	else
		return (2);
}

std::string		realpath_to_cpp_string(const char *path)
{
	char	*tmp = realpath(path, NULL);
	std::string	realpath = tmp;
	if (tmp)
		free(tmp);
	return (realpath);
}

std::string		itoa_cpp(int nb)
{
	std::stringstream	out;

	out << nb;
	return (out.str());
}
