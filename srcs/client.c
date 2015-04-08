/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/19 17:47:30 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/02 11:17:59 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../includes/libft.h"

static void	usage(char *str)
{
	ft_printf("Usage: %s <addr> <port>\n", str);
	exit (-1);
}

static void error_display(char *str)
{
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	exit(-1);
}

static int			create_client(char *addr, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if(connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error_display("connect()");
	return (sock);
}

static void execcmd(char *buf, int sock)
{
	if(strcmp("quit", buf) == 0)
	{
		close(sock);
		exit(0);
	}
}

int			main(int ac, char ** av)
{
	int					port;
	int					sock; // server socket
	int 				n;
	int 				ret;
	char				buf[1024];

	if (ac != 3)
		usage(av[0]);
	port = ft_atoi(av[2]);
	if ((sock = create_client(av[1], port))== -1)
		error_display("sock()");
	while(1)
	{
		ft_putstr("\x1B[31mft_p> \x1B[0m");
		ret = read(0, buf, 1024);
		if((n = send(sock, buf, ft_strlen(buf)-1, 0)) < 0)
			error_display("send()");
		else if (n > 0)
		{
			ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", n, ft_strsub(buf, 0, ft_strlen(buf)-1));
			while ((n = recv(sock, buf, 1023, 0)))
			{
				if(n < 2)
					break;
				buf[n] = '\0';
				ft_printf("\x1B[33mreceived %d bytes: [%s]\x1B[0m\n", n, buf);
				execcmd(buf, sock);
				ft_bzero(buf, 1024);
			}
			if (n < 0)
				error_display("recv()");
			
		}	
	}
	close(sock);
	return (0);
}