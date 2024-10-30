/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:05:34 by adahroug          #+#    #+#             */
/*   Updated: 2024/10/30 10:09:16 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

typedef struct s_data
{
	char **cmd_args;
	int args;
	char *input;
	char *pwd;
	char *cd;
	
} t_data;


int ft_strlen(const char *str);
void free_allocated(t_data *p);
void	ft_strcpy(char *dest, const char *src);
int	count_words(const char *str, char c);
char	**ft_split(char const *s, char c);
int ft_strcmp(char *s1, char *s2);
char	*word_dup(const char *str, int start, int finish);
char *pwd(t_data *p);
void build_in(t_data *p);
void free_buildin(t_data *p);
void read_command_line(t_data *p);











#endif
