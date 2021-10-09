/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stiffiny <stiffiny@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 17:21:51 by stiffiny          #+#    #+#             */
/*   Updated: 2021/10/09 17:58:06 by stiffiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include "pipex.h"

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
	{
		perror(file);
		return (3);
	}
	dup2(fd_input, STDIN_FILENO);
	close(fd_input);
	dup2(fd_output, STDOUT_FILENO);
	return (exec_cmd(cmd, envp));
}

int	pipex_output(char **envp, char *file, char *cmd, int fd_input)
{
	int	fd_output;

	fd_output = open(file, O_WRONLY);
	if (fd_output == -1)
	{
		perror(file);
		return (3);
	}
	if (dup2(fd_input, STDIN_FILENO) == -1)
	{
		printf("Dup2 error STDIN\n");
	}
	if (dup2(fd_output, STDOUT_FILENO) == -1)
	{
		printf("Dup2 error STOUT\n");
	}
	return (exec_cmd(cmd, envp));
}

// Error handling for child process
int	pipex(char *argv[], char *envp[])
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		return (perror_exit(1, "Problem opening a pipe"));
	pid = fork();
	if (pid == -1)
		return (perror_exit(2, "Error forking process"));
	if (pid == 0)
	{
		close(fd[0]);
		pipex_input(envp, argv[1], argv[2], fd[1]);
	}
	if (pid > 0)
	{
		close(fd[1]);
		wait(0);
		pipex_output(envp, argv[4], argv[3], fd[0]);
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 5)
	{
		write(2, "Wrong number of arguments\n", 26);
		return (1);
	}
	pipex(argv, envp);
	return (0);
}
