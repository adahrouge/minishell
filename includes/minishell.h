/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:05:34 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/24 15:37:27 by adahroug         ###   ########.fr       */
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
	char *name;
	char *value;
} t_export;

char	*ft_strdup(const char *s);
int ft_strchr(char *str, int c);
int	ft_isalnum(int c);
int	ft_isalpha(int c);
int ft_strlen(const char *str);
int ft_strcmp(char *s1, char *s2);
int	count_words(const char *str, char c);
void free_allocated(t_data *p);
char *ft_strcpy(char *dest, const char *src);
void build_in(t_data *p, t_export **head);
void loop(t_data *p, t_export **head);
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
int	compare_chars(const char *s1, const char *s2, size_t n);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
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

int export(t_export *head);
int export_main(t_data *p, t_export **head);
int unset(char *str, t_export **head);
t_export *merged_sorted_list(t_export *head, t_export *head2);
t_export *sort_list(t_export *head);
t_export *populate_list(char **environ);
t_export *create_my_list(char **environ, int i);
void check_for_populate(t_export **head, t_export **ptr, t_export *newnode);
void free_list(t_export *head);
void print_variables(t_export *tmp, int i);
int env(t_export *head);
char **copy_environ(char **environ);
int free_my_environ(char **my_environ, int i);
int arrange_export(t_export *head);
char *my_getenv(char *name, t_export *head);
void process_export_args(char *arg, t_export **head);
void process_export_empty_arg(char *arg, t_export **head);
t_export *create_empty_arg(char *arg);
int process_export_full_arg(char *arg, t_export **head);
int update_arg_in_list(char *var_name, char *var_value, t_export **head_ref);
void free_new_node(t_export *newnode);
t_export *create_node(char *var_name, char *var_value);
int check_arg_export(char *arg);
int free_environ(char **my_environ);
int modify_value(t_export *temp, char *var_value);
int ft_unset_all(char *input, t_export **head);


#endif 
