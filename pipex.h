#ifndef PIPEX_H
#define PIPEX_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "libft.h"
int	ft_find_substr(char *str, char *substr);
char	*ft_strjoin_del(char const *s1, char const *s2, char del);
char	**free_arr(char **arr);
#endif
