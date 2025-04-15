/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 20:24:18 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/15 18:17:20 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirections(t_data *p, t_export **head)
{
	char	**cmd_args;
	pid_t	pid;
	int		status;

	status = 0;
	cmd_args = parse_rd(p);
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free_2d_array(cmd_args);
		return ;
	}
	else if (pid == 0)
	{
		handle_child_rd(p, head, cmd_args);
		exit(127);
	}
	else
		handle_parent_rd(p, pid, cmd_args, status);
	return ;
}

void	handle_child_rd(t_data *p, t_export **head, char **cmd_args)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	p->cmd_args = cmd_args;
	check_input_rd(cmd_args, head, &i, &fd);
	if (!cmd_args[0])
		exit(0);
	if (cmd_args[0] && is_builtin(cmd_args[0]))
	{
		build_in(p, head);
		free_2d_array(cmd_args);
	}
	else
	{
		external_commands_rd(p, head, cmd_args);
		p->exit_code = 1;
		return ;
	}
}

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
