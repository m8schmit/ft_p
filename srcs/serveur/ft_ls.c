/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/21 10:28:44 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/21 11:41:28 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

void ft_ls(int cs)
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
			usleep(100);
			ft_printf(GREENPRINT, ft_strlen(entries->d_name), entries->d_name);
			send(cs, entries->d_name, ft_strlen(entries->d_name), 0);
		}
	}
	if (closedir(ptdir) == -1)
		error_display("ERROR: closedir()");
	end_cmd(cs);
}
