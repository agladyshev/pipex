/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stiffiny <stiffiny@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 17:54:44 by stiffiny          #+#    #+#             */
/*   Updated: 2021/10/09 18:11:20 by stiffiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>

size_t	ft_strlen(const char *s);
char	**ft_split(char const *str, char c);
int		ft_find_substr(char *str, char *substr);
char	*ft_strjoin_del(char const *s1, char const *s2, char del);
char	**free_arr(char **arr);
char	*resolve_path(char **envp, char *cmd);
int		perror_exit(int code, char *message);
#endif
