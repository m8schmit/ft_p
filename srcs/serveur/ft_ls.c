/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sho <sho@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/21 10:28:44 by mschmit           #+#    #+#             */
/*   Updated: 2015/05/30 17:22:21 by sho              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

void ft_ls(t_data *data)
{
	DIR				*ptdir;
	struct dirent	*entries;
	char			pwd[1024];

	getcwd(pwd, 1024);
	ptdir = opendir(".");
	if (!ptdir)
		error_display("ERROR: opendir()");
	while ((entries = readdir(ptdir)) != NULL)
	{
		if (entries->d_name[0] != '.')
		{
			ft_strcat(data->buff, entries->d_name);
			ft_strcat(data->buff, "\n");
		}
	}
	ft_printf(GREENPRINT, data->cs, ft_strlen(data->buff), data->buff);
	send(data->cs, data->buff, ft_strlen(data->buff), 0);
	ft_bzero(data->buff, ft_strlen(data->buff));
	if (closedir(ptdir) == -1)
		error_display("ERROR: closedir()");
	end_cmd(data->cs);
}
