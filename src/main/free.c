/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 22:31:30 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/23 13:14:42 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipe(t_data *p, int size)
{
	int	i;

	if (!p || !p->pipefd)
		return ;
	i = 0;
	while (i < size)
	{
		if (p->pipefd[i])
			free(p->pipefd[i]);
		i++;
	}
	free(p->pipefd);
	p->pipefd = NULL;
}

void	free_2d_array(char **array)
{
	int	i;
	int	size;

	if (!array)
		return ;
	size = 0;
	i = 0;
	while (array[size] != NULL)
		size++;
	while (i < size)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_already_allocated(char **new_paths, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		free(new_paths[i]);
		i++;
	}
	free(new_paths);
}

int	count_tokens_2d(char **arg)
{
	int	i;

	i = 0;
	while (arg[i] != NULL)
		i++;
	return (i);
}

void	cleanup_and_exit(t_data *p, int code)
{
	if (p->input)
		free(p->input);
	exit(code);
}
