/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/21 10:28:44 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/21 13:34:36 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

void ft_ls(t_data *data)
{
	DIR				*ptdir;
	struct dirent	*entries;
	char			pwd[1024];
	int				len;

	getcwd(pwd, 1024);
	ptdir = opendir(".");
	if (!ptdir)
		error_display("ERROR: opendir()");
	while ((entries = readdir(ptdir)) != NULL)
	{
		if (entries->d_name[0] != '.')
		{
			len = ft_strlen(entries->d_name);
			usleep(100);
			ft_printf(GREENPRINT, data->cs, len, entries->d_name);
			send(data->cs, entries->d_name, ft_strlen(entries->d_name), 0);
		}
	}
	if (closedir(ptdir) == -1)
		error_display("ERROR: closedir()");
	end_cmd(data->cs);
}
