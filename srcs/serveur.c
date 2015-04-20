/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschmit <mschmit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/19 14:42:57 by mschmit           #+#    #+#             */
/*   Updated: 2015/04/20 17:30:36 by mschmit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.h"

static void error_display(char *str)
{
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	exit(-1);
}

static void	usage(char *str)
{
	ft_printf("Usage: %s <port>\n", str);
	exit (-1);
}

static void end_cmd(int cs)
{
	usleep(100);
	send(cs, "\0", 1, 0);
}

static int 		create_server(int port)
{
	int 				sock;
	struct protoent			*proto;
	struct sockaddr_in 	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1 )
		error_display("bind()");
	listen(sock, 50);
	return (sock);
}

static void ft_ls(int cs)
{
	DIR				*ptdir;
	struct dirent	*entries;
	char			pwd[1024];

	getcwd(pwd, 1024);
	ptdir = opendir(".");
	if(!ptdir)
		error_display("opendir()");
	while((entries = readdir(ptdir)) != NULL)
	{
		if (entries->d_name[0] != '.')
		{
			usleep(100);
			ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", ft_strlen(entries->d_name), entries->d_name);
			send(cs, entries->d_name, ft_strlen(entries->d_name), 0);
		}
	}	
	if(closedir(ptdir) == -1)
		error_display("closedir()");
	end_cmd(cs);
}

static void ft_pwd(int cs)
{
	char dir[1024];

	getcwd(dir, 1024);
	ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", ft_strlen(dir), dir);
	send(cs, dir, ft_strlen(dir), 0);
	end_cmd(cs);
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
	tab[1][ft_strlen(tab[1])] = '\0';
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

static void ft_cd(t_data *data, char *buf)
{
	char	**tab;
	char	*tmp;
	char			pwd[1024];

	tab = ft_strsplit(buf, ' ');
	if (strcmp(tab[0], "cd") == 0 && tab[1] == NULL)
	{
		chdir(data->root);
		ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", ft_strlen(data->root), data->root);
		send(data->cs, data->root, ft_strlen(data->root), 0);
	}
	else
	{
		tmp = ft_strtrim(tab[1]);
		if (chdir(tmp) != 0)
		{
			ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", 19, "folder don't exist.");
			send(data->cs, "folder don't exist.", 19, 0);
		}
		else if (ft_strncmp(data->root, getcwd(buf, 1024), ft_strlen(data->root)) == 0)
		{
			ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", ft_strlen(buf), buf);
			send(data->cs, buf, ft_strlen(buf), 0);
		}
		else
		{
			chdir(data->root);
			ft_printf("\x1B[32msend %d bytes: [%s]\x1B[0m\n", 14, "Out of limits.");
			send(data->cs, "Out of limits.", 14, 0);

		}
		free(tmp);
		end_cmd(data->cs);
	}
}
static void app (t_data *data)

{
	int                 r;
	int 				ret;
	char                *buf;

	buf = (char*)malloc(sizeof(char) * 1024);
	ft_bzero(buf, 1024);
	r = 0;
	ret = 0;
	while(ret == 0)
	{
		if ((r = recv(data->cs, buf, 1024, 0)) < 0)
			error_display(buf);
		buf[r] = '\0';
		ft_printf("\x1B[33mreceived %d bytes: [%s]\x1B[0m\n", r, buf);
		if (strcmp(buf, "quit") == 0)
		{
			send(data->cs, "quit",5 , 0);
			ret = 1;
		}
		else if (strcmp(buf, "pwd") == 0)
			ft_pwd(data->cs);
		else if (strcmp(buf, "ls") == 0)
			ft_ls(data->cs);
		else if (strncmp(buf, "cd", 2) == 0)
			ft_cd(data, buf);
		else if (strncmp(buf, "get", 3) == 0)
			ft_put(data->cs, buf);
		else if (strncmp(buf, "put", 3) == 0)
			ft_get(data->cs, buf);
		else
		{
			send(data->cs, "ERROR", 5, 0);
			end_cmd(data->cs);
		}
	}
	free(buf);
}

int 		main(int ac, char ** av)
{
	int 				port;
	int 				sock; // server socket
	unsigned int  		cslen;
	struct sockaddr_in 	csin;
	int 				pid;
	t_data				data;
	
	if (ac != 2)
		usage(av[0]);
	port = ft_atoi(av[1]);
	sock = create_server(port);
	getcwd(data.root, 1024);
	while((data.cs = accept(sock, (struct sockaddr*)&csin, &cslen)) > -1)
	{
		if ((pid = fork()) == -1)
			error_display("fork()");
		else if (pid == 0)
			app(&data);	
	}
	if (data.cs == -1)
		error_display("accept()");
	close (data.cs);
	close(sock);
	return (0);
}