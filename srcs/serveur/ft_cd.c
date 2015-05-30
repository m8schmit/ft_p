/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sho <sho@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/21 10:37:28 by mschmit           #+#    #+#             */
/*   Updated: 2015/05/30 17:52:11 by sho              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

static void		ft_cd_norme(t_data *data, char *tmp)
{
	ft_bzero(data->buff, ft_strlen(data->buff));
	if (chdir(tmp) != 0)
	{
		ft_printf(GREENPRINT, data->cs, 20, "folder don't exist.\n");
		send(data->cs, "ERROR: folder don't exist.\n", 27, 0);
	}
	else if (ft_strncmp(data->root, getcwd(data->buff, 1024), data->len) == 0)
	{
		data->buff[ft_strlen(data->buff)] = '\n';
		ft_printf(GREENPRINT, data->cs, ft_strlen(data->buff), data->buff);
		send(data->cs, data->buff, ft_strlen(data->buff), 0);
	}
	else
	{
		chdir(data->root);
		ft_printf(GREENPRINT, data->cs, 15, "Out of limits.\n");
		send(data->cs, "ERROR: Out of limits.\n", 22, 0);
	}
}

void			ft_cd(t_data *data, char *buf)
{
	char	**tab;
	char	*tmp;

	tab = ft_strsplit(buf, ' ');
	if (ft_strcmp(tab[0], "cd") == 0 && tab[1] == NULL)
	{
		chdir(data->root);
		ft_printf(GREENPRINT, data->cs, ft_strlen(data->root), data->root);
		send(data->cs, data->root, ft_strlen(data->root), 0);
	}
	else
	{
		tmp = ft_strtrim(tab[1]);
		ft_cd_norme(data, tmp);
		free(tmp);
		end_cmd(data->cs);
	}
}
