/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:02:08 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/14 15:23:17 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipe_arg(t_data *p, int i)
{
	int	in_quotes;
	int	len;
	int	count;

	i = 0;
	in_quotes = 0;
	len = 0;
	count = 0;
	while (p->input[i] != '\0')
	{
		handle_quotes_pipes(p->input[i], &in_quotes);
		if (!in_quotes && p->input[i] == '|')
			handle_pipe(p, &len, &count, &i);
		else
		{
			len++;
			i++;
		}
	}
	if (len > 0)
	{
		create_single_arg(p, &len, &count, i - len);
		count++;
	}
	p->store_pipe_arg[count] = NULL;
}

void	create_single_arg(t_data *p, int *len, int *count, int start)
{
	int	i;

	i = 0;
	p->store_pipe_arg[*count] = malloc((*len + 1) * sizeof(char));
	if (!p->store_pipe_arg[*count])
	{
		perror("Failed to allocate memory for pipe segment");
		exit(EXIT_FAILURE);
	}
	while (i < *len)
	{
		p->store_pipe_arg[*count][i] = p->input[start + i];
		i++;
	}
	p->store_pipe_arg[*count][*len] = '\0';
}

void	trim_pipe_args(t_data *p)
{
	int	i;

	i = 0;
	while (p->store_pipe_arg[i] != NULL)
	{
		trim_whitespaces(p->store_pipe_arg[i]);
		i++;
	}
}

void	close_all_pipes(t_data *p)
{
	int	i;

	if (!p->pipefd)
		return ;
	i = 0;
	while (i < p->nb_of_pipes)
	{
		close(p->pipefd[i][0]);
		close(p->pipefd[i][1]);
		i++;
	}
}
