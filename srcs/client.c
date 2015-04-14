/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sho <sho@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/19 17:47:30 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/14 17:20:08 by sho              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.h"

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

static long ft_file_len(int fd)
{
	struct stat	s;

	fstat(fd, &s);
	return (s.st_size);
}

static void ft_put(int cs, char *buf)
{
	char 			**tab;
	int 			fd;
	long			len;
	int 		 	ret;
	char			*name;

	tab = ft_strsplit(buf, ' ');
	ft_bzero(buf, 1024);
	tab[1] = ft_strtrim(tab[1]);
	tab[1][ft_strlen(tab[1]) - 1] = '\0';
	name = ft_strdup(tab[1]);

	if ((fd = open(name, O_RDONLY)) < 0)
	{
		ft_printf("error can't open %s.\n", name);
		send(cs, "-1", 2, 0);
		sleep(1);
	}
	else
	{
		len = ft_file_len(fd);
		ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", ft_strlen(ft_itoa(len)), ft_itoa(len));
		send(cs, ft_itoa(len), ft_strlen(ft_itoa(len)), 0);
		sleep(1);
		ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", ft_strlen(name), name);
		send(cs, name, ft_strlen(name), 0);
		usleep(100);
		while((ret = read(fd, buf, 1024)) > 0)
		{
			send(cs, buf, ret, 0);
			ft_bzero(buf, 1024);
		}
		if(ret == -1)
			error_display("read()");
	}
	
	ft_putendl("close fd...");
	close(fd);
}

static void ft_get(int sock, char *buf)
{
	int 	n;
	int 	len;
	int 	fd;
	char	*name;

	ft_bzero(buf, 1023);
	n = recv(sock, buf, 1023, 0);
	buf[n] = '\0';
	ft_printf("\x1B[33m(len)received %d bytes: [%s]\x1B[0m\n", ft_strlen(buf), buf);
	len = ft_atoi(buf);
	if(len == -1)
	{
		ft_putendl("open()");
	}
	else
	{
		ft_bzero(buf, 1023);
		n = recv(sock, buf, 1023, 0);
		buf[n] = '\0';
		ft_printf("\x1B[33m(name)received %d bytes: [%s]\x1B[0m\n", ft_strlen(buf), buf);
		name = ft_strdup(buf);
		
		if ((fd = open(name, O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1)
		{
			ft_printf("error can't create %s.\n", name);
		}
		else
		{
			while(len > 0)
			{
				ft_bzero(buf, 1023);
				if((n = recv(sock, buf, 1023, 0)) < 2)
				{
					ft_bzero(buf, 1023);
				}
				ft_printf("\x1B[33m(content)received %d bytes: [%s]\x1B[0m\n", ft_strlen(buf), buf);
				write(fd, buf, n);
				len -= n;
				ft_printf("len : %d\n", len);
			}
		}
	}
	ft_bzero(buf, 1023);
	ft_putendl("close fd...");
	close(fd);
}

static void execcmd(char *buf, int sock)
{
	if(strcmp("quit", buf) == 0)
	{
		close(sock);
		exit(0);
	}
	else if (strncmp(buf, "get", 3) == 0)
		ft_get(sock, buf);
	else if (strncmp(buf, "put", 3) == 0)
		ft_put(sock, buf);
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
	ft_bzero(buf, 1024);
	while(1)
	{
		ft_putstr("\x1B[31mft_p> \x1B[0m");
		ret = read(0, buf, 1024);
		// buf[ret] = '\0';
		if((n = send(sock, buf, ft_strlen(buf)-1, 0)) < 0)
			error_display("send()");
		else if (n > 0)
		{
			ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", n, ft_strsub(buf, 0, ft_strlen(buf)-1));
			if(strncmp(buf, "put", 3) == 0 
				|| strncmp(buf, "get", 3) == 0)
				execcmd(buf, sock);
			else
			{
				while ((n = recv(sock, buf, 1023, 0)) > 1)
				{
					if(n < 2)
						break;
					buf[n] = '\0';
					ft_printf("\x1B[33mreceived %d bytes: [%s]\x1B[0m\n", n, buf);
					if (strcmp(buf, "quit") == 0)
						execcmd(buf, sock);
					ft_bzero(buf, 1024);
				}
			if (n < 0)
				error_display("recv()");
			}	
		}	
	}
	close(sock);
	return (0);
}