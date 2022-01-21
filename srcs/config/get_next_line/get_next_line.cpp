/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdussert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 11:31:33 by jdussert          #+#    #+#             */
/*   Updated: 2019/11/18 16:11:29 by jdussert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.hpp"

int	ft_free(char **stock, char **buffer, int ret)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
	if ((ret == 0 || ret == -1) && *stock)
	{
		free(*stock);
		*stock = NULL;
	}
	return (ret);
}

int	ft_stock(char **line, char **stock, char **buffer)
{
	int		len;
	char	*tmp;

	free(line[0]);
	line[0] = NULL;
	if (ft_strchr(*stock, '\n'))
	{
		if (!(tmp = ft_substr(*stock, 0, ft_strlen(*stock))))
			return (ft_free(stock, buffer, -1));
		free(*stock);
		*stock = NULL;
		len = ft_strchr(tmp, '\n') - tmp;
		if (!(line[0] = ft_substr(tmp, 0, len)))
			return (ft_free(stock, buffer, -1));
		if (!(*stock = ft_substr(ft_strchr(tmp, '\n') + 1, 0, ft_strlen(tmp))))
			return (ft_free(stock, buffer, -1));
		free(tmp);
		tmp = NULL;
		return (1);
	}
	if (!(line[0] = ft_substr(*stock, 0, ft_strlen(*stock))))
		return (ft_free(stock, buffer, -1));
	free(*stock);
	*stock = NULL;
	return (0);
}

int	ft_restock(char **line, char **stock, char **buffer)
{
	int		len;
	char	*tmp;

	if (!(tmp = ft_substr(line[0], 0, ft_strlen(line[0]))))
		return (ft_free(stock, buffer, -1));
	free(line[0]);
	line[0] = NULL;
	len = ft_strchr(tmp, '\n') - tmp;
	if (!(*stock = ft_substr(ft_strchr(tmp, '\n') + 1, 0, ft_strlen(tmp))))
		return (ft_free(stock, buffer, -1));
	if (!(line[0] = ft_substr(tmp, 0, len)))
		return (ft_free(stock, buffer, -1));
	free(tmp);
	tmp = NULL;
	return (1);
}

int	ft_read(int fd, char **line, char **stock, char **buffer)
{
	int		ret;
	char	*tmp;

	while ((ret = read(fd, *buffer, BUFFER_SIZE)))
	{
		buffer[0][ret] = '\0';
		if (ret == -1)
			return (ft_free(stock, buffer, -1));
		if (!(tmp = ft_substr(line[0], 0, ft_strlen(line[0]))))
			return (ft_free(stock, buffer, -1));
		free(line[0]);
		line[0] = NULL;
		if (!(line[0] = ft_strjoin(tmp, *buffer)))
			return (ft_free(stock, buffer, -1));
		free(tmp);
		tmp = NULL;
		if (ft_strchr(line[0], '\n'))
			if (ft_restock(&line[0], stock, buffer) == 1)
				break ;
	}
	return (ret);
}

int	get_next_line(int fd, char **line)
{
	static char	*stock = NULL;
	char		*buffer;
	int			ret;
	int			s;

	if (fd < 0 || line == NULL || BUFFER_SIZE < 1)
		return (-1);
	if ((line[0] = static_cast<char*>(ft_calloc(BUFFER_SIZE + 1, sizeof(line[0])))) == NULL)
		return (-1);
	if ((buffer = static_cast<char*>(ft_calloc(BUFFER_SIZE + 1, sizeof(buffer)))) == NULL)
		return (ft_free(&stock, &buffer, -1));
	if (stock != NULL)
		if ((s = ft_stock(&line[0], &stock, &buffer)) != 0)
			return (ft_free(&stock, &buffer, s));
	ret = ft_read(fd, &line[0], &stock, &buffer);
	if (ret < 0)
		return (ft_free(&stock, &buffer, -1));
	if (ret == 0)
		return (ft_free(&stock, &buffer, 0));
	return (ft_free(&stock, &buffer, 1));
}
