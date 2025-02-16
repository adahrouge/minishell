/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 20:52:09 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/16 16:51:53 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_command(t_data *p, int i)
{
	dup2(p->pipefd[i][1], STDOUT_FILENO); // STDOUT Becomes the write end of the pipe
	close(p->pipefd[i][0]); //close read end
	close(p->pipefd[i][1]); // close write end
}

void	middle_commands(t_data *p, int i)
{
	dup2(p->pipefd[i - 1][0], STDIN_FILENO);
	dup2(p->pipefd[i][1], STDOUT_FILENO);
	close(p->pipefd[i - 1][0]);
	close(p->pipefd[i - 1][1]);
	close(p->pipefd[i][0]);
	close(p->pipefd[i][1]);
}

void	last_command(t_data *p, int i)
{
	dup2(p->pipefd[i - 1][0], STDIN_FILENO);
	close(p->pipefd[i - 1][0]);
	close(p->pipefd[i - 1][1]);
}

void	execute_command_pipes(t_data *p, t_export *head, int i)
{
	char	**envp;
	char	**cmd_args;

	envp = convert_list_to_array(head);
	cmd_args = split_cmd_quoted(p->store_pipe_arg[i]);
	if (!cmd_args || !cmd_args[0])
	{
		free_2d_array(envp);
		if (cmd_args)
			free_2d_array(cmd_args);
		p->exit_code = 127;
		exit(EXIT_FAILURE);
	}
	if (p->correct_path)
		execve(p->correct_path, cmd_args, envp);
	perror("execve failed");
	p->exit_code = 1;
	if (p->correct_path)
		free(p->correct_path);
	free_2d_array(envp);
	free_2d_array(cmd_args);
	exit(EXIT_FAILURE);
}

void	handle_child(t_data *p, t_export *head, int i)
{
	if (i == 0 && p->num_commands > 1)
		first_command(p, i);
	else if (i == p->num_commands - 1)
		last_command(p, i);
	else
		middle_commands(p, i);
	p->cmd_args = split_cmd_quoted(p->store_pipe_arg[i]);
	if (is_builtin(p->cmd_args[0]))
	{
		build_in(p, &head);
		exit(p->exit_code);
	}
	else
	{
		create_path_pipes(p, head, i);
		execute_command_pipes(p, head, i);
	}
	exit(p->exit_code);
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
