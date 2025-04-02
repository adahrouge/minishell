/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:19:11 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/02 21:07:37 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	create_pipes(t_data *p)
{
	int	i;

	p->pipefd = malloc(sizeof(int *) * (p->nb_of_pipes));
	if (!p->pipefd)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < p->nb_of_pipes)
	{
		p->pipefd[i] = malloc(2 * sizeof(int));
		if (!p->pipefd[i])
		{
			perror("malloc failed for pipefd[i]");
			free_pipe(p, i);
			return ;
		}
		if ((pipe(p->pipefd[i]) == -1))
		{
			perror("piping didnt work");
			free_pipe(p, i);
			return ;
		}
		i++;
	}
	return ;
}

void	pipe_prepare(t_data *p)
{
	parse_pipes(p);
	create_pipes(p);
	p->pids = malloc(sizeof(pid_t) * p->num_commands);
	if (!p->pids)
		exit(EXIT_FAILURE);
}

void	pipe_wait_loop(t_data *p)
{
	int	j;
	int status = 0;
	pid_t w;
	j = 0;
	while (j < p->num_commands)
	{
		w = waitpid(p->pids[j], &status, 0);
		if (w == -1)
			perror("waitpid failed");
		j++;
	}

}

void	pipe_cleanup(t_data *p)
{
	free(p->pids);
	free_pipe(p, p->nb_of_pipes);
	free_2d_array(p->store_pipe_arg);
	if (p->full_path_pipe)
	{
		free(p->full_path_pipe);
		p->full_path_pipe = NULL;
	}
}

void	pipes(t_data *p, t_export *head)
{
	pipe_prepare(p);
	pipe_fork_loop(p, head);
	pipe_wait_loop(p);
	pipe_cleanup(p);
}
