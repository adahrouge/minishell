/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 20:52:09 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/19 16:06:13 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void first_command(t_data *p, int i)
{
	dup2(p->pipefd[i][1], STDOUT_FILENO); // STDOUT Becomes the write end of the pipe
	close(p->pipefd[i][0]);
	close(p->pipefd[i][1]);
}
void middle_commands(t_data *p, int i)
{
	dup2(p->pipefd[i - 1][0], STDIN_FILENO);
	dup2(p->pipefd[i][1], STDOUT_FILENO);
	close(p->pipefd[i - 1][0]);
	close(p->pipefd[i - 1][1]);
	close(p->pipefd[i][0]);
	close(p->pipefd[i][1]);
}
void last_command(t_data *p, int i)
{
	dup2(p->pipefd[i - 1][0], STDIN_FILENO);
	close(p->pipefd[i - 1][0]);
	close(p->pipefd[i - 1][1]);
}
void execute_command_pipes(t_data *p, t_export *head, int i)
{
	char **envp;
	char **cmd_args;
	
	envp = convert_list_to_array(head);
	cmd_args = ft_split(p->store_pipe_arg[i], ' ');
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
	if (p->correct_path)
		free(p->correct_path);
	free_2d_array(envp);
	free_2d_array(cmd_args);
	exit(EXIT_FAILURE);
}
