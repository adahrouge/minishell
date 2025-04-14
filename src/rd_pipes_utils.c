/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_pipes_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:18:25 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/14 17:24:10 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_error_exit(char *msg, t_data *p)
{
	if (msg)
		perror(msg);
	free_split(p->cmd_args);
	exit(1);
}

void	pipe_fork_loop_rd(t_data *p, t_export **head)
{
	int	i;

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
			handle_parent(p, i);
		i++;
	}
}

void	handle_child_rd_pipes(t_data *p, t_export **head, int i)
{
	if (i == 0 && p->num_commands > 1)
		first_command(p, i);
	else if (i == p->num_commands - 1)
		last_command(p, i);
	else
		middle_commands(p, i);
	p->cmd_args = split_cmd_quoted(p->store_pipe_arg[i]);
	setup_redirections_in_child(p, head);
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

void	execute_rd_pipes(t_data *p, t_export **head, int i)
{
	char	**envp;

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

void	setup_redirections_in_child(t_data *p, t_export **head)
{
	int		i;
	int		fd;
	char	**args;

	i = 0;
	args = p->cmd_args;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
			handle_out(args, p, &i, &fd);
		else if (ft_strcmp(args[i], "<") == 0)
			handle_in(args, p, &i, &fd);
		else if (ft_strcmp(args[i], "<<") == 0)
			heredoc_pipes(p, *head, &i, fd);
		else
			i++;
	}
}
