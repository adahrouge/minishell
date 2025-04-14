/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_pipes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:05:39 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/14 15:24:26 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_rd(t_data *p, t_export **head)
{
	(void)head;
	parse_pipe_arg(p);
	create_pipe_arg(p, 0);
	trim_pipe_args(p);
	p->num_commands = p->nb_of_pipes + 1;
	create_pipes(p);
	p->pids = malloc(sizeof(pid_t) * p->num_commands);
	if (!p->pids)
		exit(EXIT_FAILURE);
	pipe_fork_loop_rd(p, head);
	pipe_wait_loop(p);
	pipe_cleanup_rd(p);
}

void	remove_redir_tokens(t_data *p, int i)
{
	int	j;

	j = i;
	free(p->cmd_args[i]);
	free(p->cmd_args[i + 1]);
	while (p->cmd_args[j + 2])
	{
		p->cmd_args[j] = p->cmd_args[j + 2];
		j++;
	}
	p->cmd_args[j] = NULL;
}

void	error_fork_heredoc(char **cmd_args, char *delimiter, int *pipefd)
{
	perror("couldnt fork in heredoc");
	free(delimiter);
	close(pipefd[0]);
	close(pipefd[1]);
	free_2d_array(cmd_args);
	return ;
}

void	error_pipe_heredoc(char **cmd_args, char *delimiter)
{
	perror("couldnt create pipe in heredoc");
	free(delimiter);
	free_2d_array(cmd_args);
	return ;
}

void	pipe_cleanup_rd(t_data *p)
{
	free(p->pids);
	free_pipe(p, p->nb_of_pipes);
	free_2d_array(p->store_pipe_arg);
}
