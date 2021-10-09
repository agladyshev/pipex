/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stiffiny <stiffiny@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 17:21:51 by stiffiny          #+#    #+#             */
/*   Updated: 2021/10/09 17:30:59 by stiffiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include "pipex.h"

char	*resolve_path(char **envp, char *cmd)
{
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	path = 0;
	while (envp[i] && ft_find_substr(envp[i], "PATH") != 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i] && path == 0)
	{
		path = ft_strjoin_del(paths[i], cmd, '/');
		if (access(path, X_OK))
		{
			free(path);
			path = 0;
		}
		i++;
	}
	free_arr(paths);
	return (path);
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
	{
		perror(file);
		return (3);
	}
	printf("Input file opened %d\n", fd_input);
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
	printf("Output file opened %d\n", fd_output);
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

int	pipex(char *file1, char *cmd1, char *cmd2, char *file2, char *envp[])
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
	{
		perror("Problem opening a pipe");
		return (1);
	}
	printf("Pipe opened: %d %d\n", fd[0], fd[1]);
	pid = fork();
	if (pid == -1)
	{
		perror("Error forking process");
		return (2);
	}
	if (pid == 0)
	{
		close(fd[0]);
		pipex_input(envp, file1, cmd1, fd[1]);
	}
	if (pid > 0)
	{
		close(fd[1]);
		wait(0);
		// Error handling for child process
		pipex_output(envp, file2, cmd2, fd[0]);
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
	pipex(argv[1], argv[2], argv[3], argv[4], envp);
	return (0);
}
