/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/21 10:37:28 by mschmit           #+#    #+#             */
/*   Updated: 2015/05/04 14:30:23 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

static void		ft_cd_norme(t_data *data, char *buf, char *tmp)
{
	if (chdir(tmp) != 0)
	{
		ft_printf(GREENPRINT, data->cs, 19, "folder don't exist.");
		send(data->cs, "ERROR: folder don't exist.", 26, 0);
	}
	else if (ft_strncmp(data->root, getcwd(buf, 1024), data->len) == 0)
	{
		ft_printf(YELLOWPRINT, ft_strlen(buf), buf);
		send(data->cs, buf, ft_strlen(buf), 0);
	}
	else
	{
		chdir(data->root);
		ft_printf(GREENPRINT, data->cs, 14, "Out of limits.");
		send(data->cs, "ERROR: Out of limits.", 21, 0);
	}
}

void			ft_cd(t_data *data, char *buf)
{
	char	**tab;
	char	*tmp;

	tab = ft_strsplit(buf, ' ');
	if (strcmp(tab[0], "cd") == 0 && tab[1] == NULL)
	{
		chdir(data->root);
		ft_printf(GREENPRINT, data->cs, ft_strlen(data->root), data->root);
		send(data->cs, data->root, ft_strlen(data->root), 0);
	}
	else
	{
		tmp = ft_strtrim(tab[1]);
		ft_cd_norme(data, buf, tmp);
		free(tmp);
		end_cmd(data->cs);
	}
}
