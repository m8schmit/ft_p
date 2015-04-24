/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/21 10:25:47 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/24 11:32:11 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

static void	app_norme(t_data *data, char *buf)
{
	if (strcmp(buf, "pwd") == 0)
		ft_pwd(data);
	else if (strcmp(buf, "ls") == 0)
		ft_ls(data);
	else if (strncmp(buf, "cd", 2) == 0)
		ft_cd(data, buf);
	else if (strncmp(buf, "get ", 4) == 0)
		ft_put(data->cs, buf);
	else if (strncmp(buf, "put ", 4) == 0)
		ft_get(data->cs, buf);
	else if (strncmp(buf, "help", 4) == 0)
		ft_bzero(buf, 1024);
	else
	{
		send(data->cs, "ERROR", 5, 0);
		end_cmd(data->cs);
	}
}

void		app(t_data *data)

{
	int			r;
	int			ret;
	char		*buf;

	buf = (char*)malloc(sizeof(char) * 1024);
	ft_bzero(buf, 1024);
	r = 0;
	ret = 0;
	while (ret == 0)
	{
		if ((r = recv(data->cs, buf, 1024, 0)) < 0)
			error_display(buf);
		buf[r] = '\0';
		ft_printf(YELLOWPRINT, data->cs, r, buf);
		if (strcmp(buf, "quit") == 0)
		{
			send(data->cs, "quit", 5, 0);
			ret = 1;
		}
		else
			app_norme(data, buf);
	}
	free(buf);
}
