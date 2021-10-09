#include "pipex.h"
#include <errno.h>
#include <string.h>


int	ft_find_substr(char *str, char *substr)
{
	if (!str || !substr)
		return (-2);
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i] != 0)
	{
		j = 0;
		while (str[i + j] == substr[j])
		{
			j++;
			if (substr[j] == 0)
				return (i);
		}
		i++;
	}
	return (-1);
}

char	*ft_strjoin_del(char const *s1, char const *s2, char del)
{
	int		i;
	int		j;
	char	*p;

	i = 0;
	j = 0;
	if (s1 == 0 || s2 == 0)
		return (0);
	p = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	if (p == 0)
		return (0);
	while (s1[j] != 0)
	{
		p[i] = s1[j];
		i++;
		j++;
	}
	j = 0;
	p[i++ + j] = del;
	while (s2[j] != 0)
	{
		p[i + j] = s2[j];
		j++;
	}
	p[i + j] = 0;
	return (p);
}

char	**free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return ((char **)0);
}

char 	*resolve_path(char **envp, char *cmd)
{
	int	i;
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
	char **args;
	char *path;
	
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

int pipex(char *file1, char *cmd1, char *cmd2, char *file2, char *envp[]) {
	int fd[2];
	int pid;
	int file;
	char **argv;

	argv = 0;

	if (pipe(fd) == -1) {
		perror("pipe");
		return (1);
	}
	printf("Pipe opened: %d %d\n", fd[0], fd[1]);
	pid = fork();
	if (pid == -1) {
		// Error handling for fork error
		return (2);
	}


	if (pid == 0) {
		printf("Child process\n");
		close(fd[0]);


		file = open(file1, O_RDONLY);
		if (file == -1) {
			perror(file1);
			return (3);
		}
		printf("Input file opened %d\n", file);
		dup2(file, STDIN_FILENO);
		close(file);
		dup2(fd[1], STDOUT_FILENO);
		exec_cmd(cmd1, envp);


	}
	if (pid > 0) {
		printf("Parent process\n");
		close(fd[1]);
		wait(0);
		// Error handling for child process
		

		file = open(file2, O_WRONLY);
		if (file == -1) {
			perror(file2);
			return (3);
		}
		printf("Output file opened %d\n", file);
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			printf("Dup2 error STDIN\n");
		}
		if (dup2(file, STDOUT_FILENO) == -1)
		{
			printf("Dup2 error STOUT\n");
		}
		exec_cmd(cmd2, envp);


	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[]) {
	if (argc != 5) {
		write(2, "Wrong number of arguments\n", 26);
		return (1);
	}
	pipex(argv[1], argv[2], argv[3], argv[4], envp);
	return (0);
}

