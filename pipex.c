/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stiffiny <stiffiny@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 17:21:51 by stiffiny          #+#    #+#             */
/*   Updated: 2021/10/09 18:38:51 by stiffiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include "pipex.h"

int	error_exit(int code, char *message)
{
	write(2, message, ft_strlen(message));
	return (code);
}

int	exec_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	path = resolve_path(envp, args[0]);
	if (!path)
	{
		perror(args[0]);
		return (1);
	}
	if (execve(path, args, envp) == -1)
	{
		perror(args[0]);
		return (1);
	}
	return (0);
}

int	pipex_input(char **envp, char *file, char *cmd, int fd_output)
{
	int	fd_input;

	fd_input = open(file, O_RDONLY);
	if (fd_input == -1)
		return (perror_exit(5, file));
	if (dup2(fd_input, STDIN_FILENO) == -1)
		return (perror_exit(6, "dup2"));
	if (close(fd_input) == -1)
		return (perror_exit(7, "close"));
	if (dup2(fd_output, STDOUT_FILENO) == -1)
		return (perror_exit(8, "dup2"));
	return (exec_cmd(cmd, envp));
}

int	pipex_output(char **envp, char *file, char *cmd, int fd_input)
{
	int	fd_output;

	fd_output = open(file, O_WRONLY);
	if (fd_output == -1)
		return (perror_exit(5, file));
	if (dup2(fd_input, STDIN_FILENO) == -1)
		return (perror_exit(6, "dup2"));
	if (dup2(fd_output, STDOUT_FILENO) == -1)
		return (perror_exit(6, "dup2"));
	return (exec_cmd(cmd, envp));
}

int	main(int argc, char *argv[], char *envp[])
{
	int	fd[2];
	int	pid;

	if (argc != 5)
		return (error_exit(1, "Wrong number of arguments\n"));
	if (pipe(fd) == -1)
		return (perror_exit(2, "Problem opening a pipe"));
	pid = fork();
	if (pid == -1)
		return (perror_exit(3, "Error forking process"));
	if (pid == 0)
	{
		close(fd[0]);
		pipex_input(envp, argv[1], argv[2], fd[1]);
	}
	if (pid > 0)
	{
		close(fd[1]);
		if (wait(0) == -1)
			return (perror_exit(4, "Child process error"));
		pipex_output(envp, argv[4], argv[3], fd[0]);
	}
	return (0);
}
