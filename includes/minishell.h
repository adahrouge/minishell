/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:05:34 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/10 19:43:57 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <signal.h>
# include <stddef.h>
# include <fcntl.h>
# include <stdbool.h>
# define STDERR 2

extern volatile sig_atomic_t	g_signal_received;

typedef struct s_data
{
	char	**cmd_args;
	char	*input;
	char	*pwd;
	char	*cd;
	char	**commands;
	char	***command_args;
	int		args;
	int		pipe_count;
	int		command_count;
	char	*value;
	int		exit_code;
	int		**pipefd;
	char	**cmds_pipe;
	int		nb_of_pipes;
	char	**store_pipe_arg;
	int		arg_count_pipes;
	int		num_commands;
	char	*full_path_pipe;
	char	**pipe_args;
	char	*correct_path;
	char	*command_name;
	int		*pids;
}	t_data;

// typedef struct s_sig
// {
// 	pid_t pid;
// 	int sigint;
// 	int sigquit;
// }	t_sig;

//extern t_sig global_signal;

typedef struct s_export
{
	struct s_export	*previous;
	struct s_export	*next;
	char			*data;
	char			*name;
	char			*value;
}	t_export;

typedef struct s_expansion
{
	int	i;
	int	dest;
}	t_expansion;

typedef struct s_quotes
{
	int	in_double;
	int	in_single;
}	t_quotes;

//main functions + builtins
void		build_in(t_data *p, t_export **head);
void		loop(t_data *p, t_export **head);
void		read_command_line(t_data *p);
int			pwd(t_data *p);
int			cd(t_data *p, int value);
int			is_builtin(char *str);
int			read_line(t_data *p);
void		handle_command_execution(t_data *p, t_export **head);
int			check_loop_result(t_data *p);
void		handle_pipe_or_command(t_data *p, t_export **head);

//signals
void		sigint_handler(int signum);
void		sigquit_handler(int signum);
void		setup_signal_handlers(void);

//checking condition
int			input_is_null(t_data *p);
int			input_is_backslash(t_data *p);
int			input_is_exit(t_data *p);
int			input_is_space(t_data *p);
int			input_is_redirect(t_data *p);
int			input_is_slash(t_data *p);
int			input_is_dash(t_data *p);
int			input_is_and(t_data *p);
int			input_is_clear(t_data *p);
int			input_contains_pipe(t_data *p);

//lib_ft
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
int			compare_chars(const char *s1, const char *s2, size_t n);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strdup(const char *s);
int			ft_strchr(char *str, int c);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_strlen(const char *str);
int			ft_strcmp(char *s1, char *s2);
int			count_words(const char *str, char c);
char		*ft_strcpy(char *dest, const char *src);
char		**ft_split(char *s, char c);
char		*word_dup(const char *str, int start, int finish);
void		ft_putstr_fd(char *s, int fd);
char		*ft_itoa(int n);

//echo
int			echo(t_data *p, t_export *head);
int			check_echo(char *arg);
void		parse_echo(char *arg, t_export *head);
int			handle_dollar(char *arg, t_export *head);
int			handle_quotations(char *arg, int *i, t_export *head);
int			handle_single_quotes(char *arg);
int			handle_double_quotes(char *arg, t_export *head);
int			expand_variable(char *arg, t_export *head);
void		echo_print_arg(char *arg, t_export *head);
int			echo_conditions(t_data *p, int *i, int *no_newline);
int			exit_status(t_data *p);
void		trim_arg(t_data *p, t_export **head);
int			toggle_quotes(char c, t_quotes *quotes);
int			handle_dollar_expansion(char *arg, int i,
				t_quotes *quotes, t_export *head);

//export env unset
int			export(t_export *head);
int			export_main(t_data *p, t_export **head);
int			unset(char *str, t_export **head);
t_export	*merged_sorted_list(t_export *head, t_export *head2);
t_export	*sort_list(t_export *head);
t_export	*populate_list(char **environ);
t_export	*create_my_list(char **environ, int i);
void		check_for_populate(t_export **head,
				t_export **ptr, t_export *newnode);
void		free_list(t_export *head);
void		print_variables(t_export *tmp, int i);
int			env(t_export *head);
char		**copy_environ(char **environ);
int			free_my_environ(char **my_environ, int i);
int			arrange_export(t_export *head);
char		*my_getenv(char *name, t_export *head);
int			process_export_args(char *arg, t_export **head);
void		process_export_empty_arg(char *arg, t_export **head);
t_export	*create_empty_arg(char *arg);
int			process_export_full_arg(char *arg, t_export **head);
int			update_arg_in_list(char *var_name,
				char *var_value, t_export **head_ref);
void		free_new_node(t_export *newnode);
t_export	*create_node(char *var_name, char *var_value);
int			check_arg_export(char *arg);
int			free_environ(char **my_environ);
int			modify_value(t_export *temp, char *var_value);
int			ft_unset_all(char *input, t_export **head);

//execution
void		external_commands(t_data *p, t_export *head, char *path_env);
void		execute_command(char *full_path, t_data *p, t_export *head);
char		*create_full_path(t_data *p, char **new_paths);
void		copy_paths(char **paths, char **new_paths);
char		**create_new_path(char **paths);
void		parent_execution(pid_t pid, int status, t_data *p, char *full_path);
void		free_external_commands(char **paths, char **new_paths);
int			size_list(t_export *head);

//pipes
void		close_all_pipes(t_data *p);
void		create_pipes(t_data *p);
void		first_command(t_data *p, int i);
void		middle_commands(t_data *p, int i);
void		last_command(t_data *p, int i);
void		create_fork(t_data *p, t_export *head, int *i);
int			execute_command_pipes(t_data *p, t_export *head, int i);
char		**convert_list_to_array(t_export *head);
void		parse_pipe_arg(t_data *p);
void		create_pipe_arg(t_data *p, int i);
void		create_single_arg(t_data *p, int *len, int *count, int start);
void		trim_whitespaces(char *str);
void		trim_pipe_args(t_data *p);
// char *create_path_pipes(t_data *p, t_export *head, int i);
char		*parse_command(t_data *p, int i);
void		parse_pipes(t_data *p);
void		remove_all_quotes_pipes(char *str);
void		pipes(t_data *p, t_export *head);
void		handle_pipe(t_data *p, int *len, int *count, int *i);
void		handle_quotes_pipes(char c, int *in_quotes);
int			check_cmd_name_valid(t_data *p, int cmd_index, char **cmd_name);
char		**build_paths_and_newpaths(char *path_env, char *cmd_name);
char		*search_executable(t_data *p, char **new_paths, char *cmd_name);
int			build_full_path(t_data *p, char *dir, char *cmd);
char		*create_path_pipes(t_data *p, t_export *head, int cmd_index);
void		pipe_prepare(t_data *p);
void		handle_child(t_data *p, t_export *head, int i);
void		handle_parent(t_data *p, int i);
void		pipe_fork_loop(t_data *p, t_export *head);
void		pipe_wait_loop(t_data *p);
void		pipe_cleanup(t_data *p);
int			pipe_input_correct(t_data *p);
int			input_check_pipe(t_data *p, int *i,
				int *in_quotes, int *segment_len);

//debug
void		free_all(t_data *p, t_export *head, char **my_environ);
//quote_split.c
void		skip_quotes(const char *str, int *i);
int			count_tokens(const char *str);
char		*copy_token(const char *str, int start, int end);
char		**split_cmd_quoted(const char *str);
char		**split_cmd_quoted_core(const char *str,
				char **tokens, int token_count);
//free
void		free_allocated(t_data *p);
void		free_split(char **split);
void		free_2d_array(char **array);
void		free_pipe(t_data *p, int num_commands);
void		free_already_allocated(char **new_paths, int len);

//executable
int			has_slash(char *arg);
void		update_shlvl(t_export **head);
void		executable(t_data *p, t_export*head);
void		remove_quotes_args(char **args);
void		remove_quotes(char *arg);
void		executable_error(t_data *p, char *cmd, char **envp);
void		executable_main(t_data *p, t_export **head);
//expansion
char		*handle_expansion(char *oldtoken,
				t_export *head, t_expansion *exp, char *newstr);
void		expand_all_tokens(char **tokens, t_export *head);
char		*expand_single_token(char *oldtoken, t_export *head);
int			copy_value(char *dest_str, int dest, char *value);
int			parse_var_name(char *old, int *i_ptr, char *var_name);

//input is exit
void		cleanup_and_exit(t_data *p, int code);
int			exit_print_too_many(t_data *p);
void		exit_print(t_data *p, char *arg);
int			is_numeric(char *str);
int			input_is_exit(t_data *p);
int			exit_main(t_data *p);
int			count_tokens_2d(char **arg);
//redirections
void redirections(t_data *p, t_export **head);
void handle_parent_rd(t_data *p, pid_t pid, char **cmd_args, int status);
void handle_child_rd(t_data *p, t_export **head, char **cmd_args);
void rd_isinput(char **cmd_args, int *fd, int *i);
void rd_isappend(char **cmd_args, int *fd, int *i);
void rd_isoutput(char **cmd_args, int *fd, int *i);
void shift_tokens(char **cmd_args, int *i, int offset);
int	input_contains_redirection(t_data *p);
int	rd_input_correct(t_data *p);
int	next_arg_rd(char **cmd_args, int *i);
void pipe_rd(t_data *p, t_export **head);
char **parse_rd(t_data *p);
void execute_commands_rd(char *full_path, char **execve_args, char **envp, t_data *p);
void external_commands_rd(t_data *p, t_export **head, char **cmd_args);
char *create_path_rd(t_data *p, t_export *head, char *path_env);
char **create_execve_rd(char **cmd_args);

//redirections + pipes
void pipe_fork_loop_rd(t_data *p, t_export **head);
void handle_child_rd_pipes(t_data *p, t_export **head, int i);
void setup_redirections_in_child(t_data *p);
int open_file_for_output(char *op, char *filename);
void remove_redir_tokens(t_data *p, int i);
void	pipe_cleanup_rd(t_data *p);
void execute_rd_pipes(t_data *p, t_export **head, int i);

//heredoc
void rd_isheredoc(t_export *head, char **cmd_args, int *i);
void heredoc_parent(int pipefd[2], char *delimiter, char **cmd_args, int *i);
void heredoc_child(t_export *head, char *delimiter, int pipefd[2]);
void error_fork_heredoc(char **cmd_args, char *delimiter, int *pipefd);
void error_pipe_heredoc(char **cmd_args, char *delimiter);
int check_input_heredoc(char **cmd_args, int *i);

//get_next_line
char	*ft_remove_line(char *static_line);
char	*ft_get_target(char *static_line);
char	*allocate_and_read_buffer(int fd, int *bytes_read);
char	*ft_update_static(char *static_line, int fd);
char	*get_next_line(int fd);

#endif 
