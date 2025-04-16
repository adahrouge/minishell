/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 20:24:18 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/16 19:59:11 by adahroug         ###   ########.fr       */
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
void	handle_parent_rd(t_data *p, pid_t pid, char **cmd_args, int status)
{
	waitpid(pid, &status, 0);
	free_2d_array(cmd_args);
	if (WIFEXITED(status))
		p->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		p->exit_code = 128 + WTERMSIG(status);
	return ;
}
int	check_input_rd(char **cmd_args, t_export **head, int *i, int *fd)
{
	while (cmd_args[*i] != NULL)
	{
		if (ft_strcmp(cmd_args[*i], ">") == 0)
			rd_isoutput(cmd_args, fd, i);
		else if (ft_strcmp(cmd_args[*i], ">>") == 0)
			rd_isappend(cmd_args, fd, i);
		else if (ft_strcmp(cmd_args[*i], "<") == 0)
			rd_isinput(cmd_args, fd, i);
		else if (ft_strcmp(cmd_args[*i], "<<") == 0)
			handle_all_heredocs(*head, cmd_args);
		else
			(*i)++;
	}
	return (-100);
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
