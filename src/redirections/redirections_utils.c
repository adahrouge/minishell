/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 19:58:19 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/22 19:45:29 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rd_isinput(char **cmd_args, int *fd, int *i)
{
	if (!cmd_args[*i + 1])
	{
		free_2d_array(cmd_args);
		exit(1);
	}
	*fd = open(cmd_args[*i + 1], O_RDONLY);
	if (*fd < 0)
	{
		perror("open");
		free_2d_array(cmd_args);
		exit(1);
	}
	if (dup2(*fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(*fd);
		free_2d_array(cmd_args);
		exit(1);
	}
	close(*fd);
	free(cmd_args[*i]);
	free(cmd_args[*i + 1]);
	shift_tokens(cmd_args, i, 2);
}

void	rd_isappend(char **cmd_args, int *fd, int *i)
{
	if (!cmd_args[*i + 1])
	{
		free_2d_array(cmd_args);
		exit(1);
	}
	*fd = open(cmd_args[*i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd < 0)
	{
		perror("open");
		free_2d_array(cmd_args);
		exit(1);
	}
	if (dup2(*fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(*fd);
		free_2d_array(cmd_args);
		exit(1);
	}
	close(*fd);
	free(cmd_args[*i]);
	free(cmd_args[*i + 1]);
	shift_tokens(cmd_args, i, 2);
}

void	rd_isoutput(char **cmd_args, int *fd, int *i)
{
	if (!cmd_args[*i + 1])
	{
		free_2d_array(cmd_args);
		exit(1);
	}
	*fd = open(cmd_args[*i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd < 0)
	{
		close(*fd);
		free_2d_array(cmd_args);
		exit(1);
	}
	if (dup2(*fd, STDOUT_FILENO) < 0)
	{
		close(*fd);
		free_2d_array(cmd_args);
		exit(1);
	}
	close(*fd);
	free(cmd_args[*i]);
	free(cmd_args[*i + 1]);
	shift_tokens(cmd_args, i, 2);
	return ;
}

void	shift_tokens(char **cmd_args, int *i, int offset)
{
	int	j;

	j = *i;
	while (cmd_args[j + offset])
	{
		cmd_args[j] = cmd_args[j + offset];
		j++;
	}
	cmd_args[j] = NULL;
}

void	path_is_null(char **cmd_args)
{
	write(2, "command not found\n", 19);
	free_2d_array(cmd_args);
	return ;
}
