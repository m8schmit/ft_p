/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/21 10:35:38 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/21 13:28:36 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

void ft_pwd(t_data *data)
{
	char dir[1024];

	getcwd(dir, 1024);
	ft_printf(GREENPRINT, data->cs, ft_strlen(dir), dir);
	send(data->cs, dir, ft_strlen(dir), 0);
	end_cmd(data->cs);
}
