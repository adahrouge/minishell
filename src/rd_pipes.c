/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_pipes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:05:39 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/08 20:25:34 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pipe_rd(t_data *p, t_export **head)
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

void remove_redir_tokens(t_data *p, int i)
{
	int j;

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

