/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stiffiny <stiffiny@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 17:56:09 by stiffiny          #+#    #+#             */
/*   Updated: 2021/10/09 17:57:08 by stiffiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	perror_exit(int code, char *message)
{
	perror(message);
	return (code);
}

int	ft_find_substr(char *str, char *substr)
{
	int	i;
	int	j;

	if (!str || !substr)
		return (-2);
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
		p[i++] = s1[j++];
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
