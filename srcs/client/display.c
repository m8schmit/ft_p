/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/20 18:06:13 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/24 11:33:04 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.h"

void			ft_help(int sock)
{
	write(0, "HELP:\n", 6);
	write(0, "\t\x1B[1;37mls\x1B[0m -- list directory contents\n", 42);
	write(0, "\t\x1B[1;37mpwd\x1B[0m -- return working directory name\n", 49);
	write(0, "\t\x1B[1;37mget <file>\x1B[0m -- dl file from server\n", 46);
	write(0, "\t\x1B[1;37mput <file>\x1B[0m -- up file to server\n", 44);
}

static void		execcmd(char *buf, int sock)
{
	if (strcmp("quit", buf) == 0)
	{
		close(sock);
		exit(0);
	}
	else if (strncmp(buf, "get ", 4) == 0)
		ft_get(sock, buf);
	else if (strncmp(buf, "put ", 4) == 0)
		ft_put(sock, buf);
	else if (strncmp(buf, "help", 4) == 0)
	{
		ft_bzero(buf, 1024);
		ft_help(sock);
	}
	else
		ft_putendl("ERROR");
}

static void		ft_display_norme(int n, int sock, char *buf)
{
	int		error;

	error = 0;
	while ((n = recv(sock, buf, 1023, 0)) > 1)
	{
		if (n < 2)
			break ;
		buf[n] = '\0';
		ft_printf("%s\n", buf);
		if (strcmp(buf, "quit") == 0)
			execcmd(buf, sock);
		if (strncmp(buf, "ERROR", 5) == 0)
			error++;
		ft_bzero(buf, 1024);
	}
	if (n < 0)
		error_display("ERROR: recv()");
	if (error == 0)
	{
		ft_putendl("SUCCESS");
	}
}

void			ft_display(char *buf, int sock)
{
	int		ret;
	int		n;

	ft_bzero(buf, 1024);
	ft_putstr("\x1B[31mft_p> \x1B[0m");
	ret = read(0, buf, 1024);
	buf[ret] = '\0';
	if ((n = send(sock, buf, ft_strlen(buf) - 1, 0)) < 0)
		error_display("ERROR: send()");
	else if (n > 0)
	{
		if (strncmp(buf, "put", 3) == 0
			|| strncmp(buf, "get", 3) == 0
			|| strncmp(buf, "help", 4) == 0)
			execcmd(buf, sock);
		else
			ft_display_norme(n, sock, buf);
	}
}
