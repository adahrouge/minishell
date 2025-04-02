/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiprocessing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:00:35 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/02 21:13:57 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child(t_data *p, t_export *head, int i)
{
	// fprintf(stderr, "[DEBUG] Child #%d created -> command: %s\n", i, p->store_pipe_arg[i]);

	if (i == 0 && p->num_commands > 1)
		first_command(p, i);
	else if (i == p->num_commands - 1)
		last_command(p, i);
	else
		middle_commands(p, i);
	p->cmd_args = split_cmd_quoted(p->store_pipe_arg[i]);
	// fprintf(stderr, "[DEBUG] Child #%d tokens[0] = %s\n", i, p->cmd_args[0]);

	if (is_builtin(p->cmd_args[0]))
	{
		build_in(p, &head);
		exit(p->exit_code);
	}
	else
	{
		create_path_pipes(p, head, i);
		p->exit_code = execute_command_pipes(p, head, i);
	}
	return ;
}

void	handle_parent(t_data *p, int i)
{
	if (i == 0 && p->num_commands > 1)
		close(p->pipefd[0][1]);
	else if (i < p->num_commands - 1)
		close(p->pipefd[i][1]);
	else if (i > 0)
		close(p->pipefd[i - 1][0]);
}

void	pipe_fork_loop(t_data *p, t_export *head)
{
	int	i;

	// i =  p->num_commands - 1;
	i = 0;
	while (i < p->num_commands)
	{
		// // fprintf(stderr, "[DEBUG] Forking for command #%d: %s\n",
		// 	i, p->store_pipe_arg[i]);
		p->pids[i] = fork();
		if (p->pids[i] < 0)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		else if (p->pids[i] == 0)
			handle_child(p, head, i);
		else
		{
			handle_parent(p, i);
			p->full_path_pipe = NULL;
		}
		//i--;
		i++;
	}
}
