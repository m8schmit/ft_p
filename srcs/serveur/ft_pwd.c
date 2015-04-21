/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/21 10:35:38 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/21 10:36:04 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

void ft_pwd(int cs)
{
	char dir[1024];

	getcwd(dir, 1024);
	ft_printf(GREENPRINT, ft_strlen(dir), dir);
	send(cs, dir, ft_strlen(dir), 0);
	end_cmd(cs);
}
