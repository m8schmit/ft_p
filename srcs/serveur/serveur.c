/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/19 14:42:57 by mschmit           #+#    #+#             */
/*   Updated: 2015/06/01 13:38:35 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server.h"

static void			usage(char *str)
{
	ft_printf("Usage: %s <port>\n", str);
	exit (-1);
}

static int			create_server(int port)
{
	int						sock;
	struct protoent			*proto;
	struct sockaddr_in		sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error_display("ERROR: bind()");
	listen(sock, 50);
	return (sock);
}

void				forksocket(int sock)
{
	unsigned int		cslen;
	struct sockaddr_in	csin;
	int					pid;
	t_data				data;

	getcwd(data.root, 1024);
	data.len = ft_strlen(data.root);
	while ((data.cs = accept(sock, (struct sockaddr*)&csin, &cslen)) > -1)
	{
		if ((pid = fork()) == -1)
			error_display("ERROR: fork()");
		else if (pid == 0)
		{
			ft_printf("[\x1B[32mO\x1B[0m]New user [%d] connected\n", data.cs);
			app(&data);
			ft_printf("[\x1B[31mX\x1B[0m]User [%d] deconnected\n", data.cs);
		}
	}
	if (data.cs == -1)
		error_display("ERROR: accept()");
	close (data.cs);
}

int					main(int ac, char **av)
{
	int			port;
	int			sock;

	if (ac != 2)
		usage(av[0]);
	port = ft_atoi(av[1]);
	sock = create_server(port);
	ft_printf("Server now Online (port[%d]).\n", port);
	forksocket(sock);
	close(sock);
	return (0);
}
