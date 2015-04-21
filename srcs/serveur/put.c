/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/21 10:43:47 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/21 11:42:22 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

static long		ft_file_len(int fd)
{
	struct stat	s;

	fstat(fd, &s);
	return (s.st_size);
}

static void		ft_put_norme(int cs, int fd, char *name, char *buf)
{
	long			len;
	int				ret;

	len = ft_file_len(fd);
	send(cs, ft_itoa(len), ft_strlen(ft_itoa(len)), 0);
	sleep(1);
	send(cs, name, ft_strlen(name), 0);
	usleep(100);
	while ((ret = read(fd, buf, 1024)) > 0)
	{
		send(cs, buf, ret, 0);
		ft_bzero(buf, 1024);
	}
	if (ret == -1)
		error_display("ERROR: read()");
	else
		ft_putendl("SUCCESS");
}

int				ft_put(int cs, char *buf)
{
	char			**tab;
	int				fd;
	char			*name;

	tab = ft_strsplit(buf, ' ');
	ft_bzero(buf, 1024);
	if (tab[1] == NULL)
	{
		send(cs, "-1", 2, 0);
		return (-1);
	}
	tab[1] = ft_strtrim(tab[1]);
	tab[1][ft_strlen(tab[1])] = '\0';
	name = ft_strdup(tab[1]);
	if ((fd = open(name, O_RDONLY)) < 0)
	{
		ft_printf("ERROR: can't open %s.\n", name);
		send(cs, "-1", 2, 0);
		sleep(1);
	}
	else
		ft_put_norme(cs, fd, name, buf);
	close(fd);
	return (0);
}
