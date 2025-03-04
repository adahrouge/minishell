/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 20:52:09 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/28 14:57:50 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_command(t_data *p, int i)
{
	dup2(p->pipefd[i][1], STDOUT_FILENO);
	close(p->pipefd[i][0]);
	close(p->pipefd[i][1]);
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
	remove_quotes_args(cmd_args);
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

// get ls from ls - l
char	*parse_command(t_data *p, int cmd_index)
{
	char	*raw;
	char	*cmd;
	int		len;

	raw = p->store_pipe_arg[cmd_index];
	if (!raw || !raw[0])
		return (NULL);
	len = 0;
	while (raw[len] && raw[len] != ' ')
		len++;
	cmd = strndup(raw, len);
	return (cmd);
}
