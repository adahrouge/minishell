/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_pipes_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:18:25 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/08 20:37:04 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void redir_error_exit(char *msg, t_data *p)
{
	if (msg)
		perror(msg);
	free_split(p->cmd_args);
	exit(1);
}
void pipe_fork_loop_rd(t_data *p, t_export **head)
{
	int i;

    i = 0;
    while (i < p->num_commands)
    {
        p->pids[i] = fork();
        if (p->pids[i] < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (p->pids[i] == 0)
            handle_child_rd_pipes(p, head, i);
        else
            handle_parent(p, i);   // same as your normal handle_parent
        i++;
    }
}
void handle_child_rd_pipes(t_data *p, t_export **head, int i)
{
	if (i == 0 && p->num_commands > 1)
	  first_command(p, i);
	else if (i == p->num_commands - 1)
	  last_command(p, i);
	else
	  middle_commands(p, i);
  p->cmd_args = split_cmd_quoted(p->store_pipe_arg[i]);
  setup_redirections_in_child(p);
  close_all_pipes(p);
  if (p->cmd_args[0] && is_builtin(p->cmd_args[0]))
  {
	  build_in(p, head);
	  free_split(p->cmd_args);
	  exit(p->exit_code);
  }
  else
    execute_rd_pipes(p, head, i);
}
void execute_rd_pipes(t_data *p, t_export **head, int i)
{
    char **envp;
    
    envp = convert_list_to_array(*head);
    create_path_pipes(p, *head, i);
    if (p->correct_path)
    {
        execve(p->correct_path, p->cmd_args, envp);
        perror("execve failed");
    }
    free_split(envp);
    free_split(p->cmd_args);
    exit(p->exit_code);
}
void setup_redirections_in_child(t_data *p)
{
    int   i;
    int   fd;
    char **args;

    i = 0;
    args = p->cmd_args;
    while (args[i])
    {
        /* Check for ">" or ">>" */
        if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
        {
            if (!args[i + 1])
                redir_error_exit("Syntax error near >", p);

            fd = open_file_for_output(args[i], args[i + 1]);
            if (fd < 0)
                redir_error_exit("Open output failed", p);

            if (dup2(fd, STDOUT_FILENO) < 0)
                redir_error_exit("dup2 output failed", p);

            close(fd);
            remove_redir_tokens(p, i);
            /* DO NOT do i++ here, because a new token might now be at args[i] */
        }
        /* Check for "<" */
        else if (ft_strcmp(args[i], "<") == 0)
        {
            if (!args[i + 1])
                redir_error_exit("Syntax error near <", p);

            fd = open(args[i + 1], O_RDONLY);
            if (fd < 0)
                redir_error_exit("Open input failed", p);

            if (dup2(fd, STDIN_FILENO) < 0)
                redir_error_exit("dup2 input failed", p);

            close(fd);
            remove_redir_tokens(p, i);
            /* DO NOT i++ here either */
        }
        else
        {
            /* Normal token – just move on */
            i++;
        }
    }
}

int open_file_for_output(char *op, char *filename)
{
	int fd;

	if (ft_strcmp(op, ">>") == 0)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return fd;
}

void	pipe_cleanup_rd(t_data *p)
{
	free(p->pids);
	free_pipe(p, p->nb_of_pipes);
	free_2d_array(p->store_pipe_arg);
}