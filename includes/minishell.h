/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouraad <abouraad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:05:34 by adahroug          #+#    #+#             */
/*   Updated: 2024/10/31 13:23:29 by abouraad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>

typedef struct s_data
{
	char **cmd_args;
	char *input;
	char *pwd;
	char *cd;
	char **commands;
	char ***command_args;
	int args;
	int pipe_count;
	int command_count;
	
	
} t_data;


int ft_strlen(const char *str);
int ft_strcmp(char *s1, char *s2);
int	count_words(const char *str, char c);
void free_allocated(t_data *p);
void	ft_strcpy(char *dest, const char *src);
void build_in(t_data *p);
void read_command_line(t_data *p);
void sigint_handler(int signum);
void sigquit_handler(int signum);
void	free_split(char **split);
char	**ft_split(char const *s, char c);
char	*word_dup(const char *str, int start, int finish);
char *pwd(t_data *p);













#endif