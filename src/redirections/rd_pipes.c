/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_pipes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:11:07 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/22 19:45:19 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_rd(t_data *p, t_export **head)
{
	char	**all_args;
	int		std_in;

	std_in = dup(STDIN_FILENO);
	all_args = split_cmd_quoted(p->input);
	handle_all_heredocs(*head, all_args);
	free_2d_array(all_args);
	parse_pipes(p);
	create_pipes(p);
	p->pids = malloc(sizeof(pid_t) * p->num_commands);
	if (!p->pids)
		exit(EXIT_FAILURE);
	pipe_fork_loop_rd(p, head);
	pipe_wait_loop(p);
	pipe_cleanup_rd(p);
	dup2(std_in, STDIN_FILENO);
	close(std_in);
}

void	pipe_fork_loop_rd(t_data *p, t_export **head)
{
	int		i;
	char	**cmd_args;

	cmd_args = NULL;
	i = 0;
	while (i < p->num_commands)
	{
		p->pids[i] = fork();
		if (p->pids[i] < 0)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		if (p->pids[i] == 0)
			handle_child_rd_pipes(p, head, i, cmd_args);
		else
			handle_parent(p, i);
		i++;
	}
}

void	handle_child_rd_pipes(t_data *p, t_export **head,
			int index, char **cmd_args)
{
	cmd_args = split_cmd_quoted(p->store_pipe_arg[index]);
	remove_quotes_args(cmd_args);
	p->cmd_args = cmd_args;
	setup_redirections_in_child(p, head);
	if (index == 0 && p->num_commands > 1)
		first_command(p, index);
	else if (index == p->num_commands - 1)
		last_command(p, index);
	else
		middle_commands(p, index);
	close_all_pipes(p);
	if (cmd_args[0] && is_builtin(cmd_args[0]))
	{
		build_in(p, head);
		free_2d_array(cmd_args);
		exit(p->exit_code);
	}
	create_path_pipes(p, *head, index);
	if (!p->correct_path)
	{
		path_is_null(cmd_args);
		exit(127);
	}
	execve(p->correct_path, cmd_args, convert_list_to_array(*head));
	exit(127);
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
		if (ft_strcmp(args[i], "<<") == 0)
			handle_all_heredocs(*head, args);
		else if (ft_strcmp(args[i], "<") == 0)
			rd_isinput(args, &fd, &i);
		else if (ft_strcmp(args[i], ">>") == 0)
			rd_isappend(args, &fd, &i);
		else if (ft_strcmp(args[i], ">") == 0)
			rd_isoutput(args, &fd, &i);
		else
			i++;
	}
}

void	pipe_cleanup_rd(t_data *p)
{
	int	i;

	i = 0;
	free(p->pids);
	while (i < p->nb_of_pipes)
	{
		free(p->pipefd[i]);
		i++;
	}
	free(p->pipefd);
	free_2d_array(p->store_pipe_arg);
}
