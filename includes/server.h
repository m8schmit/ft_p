/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/25 17:08:19 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/21 13:31:01 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H
# define GREENPRINT "[%d]\x1B[32msend %d bytes: [%s]\x1B[0m\n"
# define YELLOWPRINT "[%d]\x1B[33mreceived %d bytes: [%s]\x1B[0m\n"
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include "libft.h"

typedef struct		s_data
{
	int				cs;
	int				len;
	char			root[1024];
}					t_data;

void				error_display(char *str);
void				ft_ls(t_data *data);
void				app (t_data *data);
void				end_cmd(int cs);
void				ft_pwd(t_data *data);
void				ft_cd(t_data *data, char *buf);
void				ft_get(int sock, char *buf);
int					ft_put(int cs, char *buf);

#endif
