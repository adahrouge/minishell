/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:05:34 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/09 17:54:33 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>


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
	char *value;
	
	
} t_data;


typedef struct s_export
{
	struct s_export *previous;
	struct s_export *next;
	char *data;
} t_export;


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
void pwd(t_data *p);
int cd(t_data *p, int value);
int input_is_null(t_data *p);
int input_is_backslash(t_data *p);
int input_is_exit(t_data *p);
int export(void);
t_export *merged_sorted_list(t_export *head, t_export *head2);
t_export *sort_list(t_export *head);
t_export *populate_list(t_export *head);
void free_list(t_export *head);
void print_exported_variables(t_export *tmp, int i);
int env(void);
int	compare_chars(const char *s1, const char *s2, size_t n);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int unset(char *str);
int input_is_space(t_data *p);
int input_is_redirect(t_data *p);
int input_is_slash(t_data *p);
int input_is_dash(t_data *p);
int input_is_and(t_data *p);
int input_is_clear(t_data *p);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int expand_variable(char *str, t_data *p);
void process_argument(char *arg, t_data *p);
int check_if_nb(char *arg);
int check_echo_input(t_data *p);
char	*ft_strjoin(char const *s1, char const *s2);
void newline_loop(void);
int parse_echo_with_quotes(t_data *p);
void dollar_and_quotations(char *arg);
void echo_count_quotes(t_data *p, int *double_quotes, int *single_quotes);
void setup_signal_handlers(void);
int echo_with_n(t_data *p, int i);
int echo(t_data *p);
int echo_wrong(t_data *p);

#endif 
