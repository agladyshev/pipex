#include "pipex.h"

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
