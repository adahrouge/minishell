/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:23:40 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/16 00:44:23 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_file_for_output(char *op, char *filename)
{
	int	fd;

	if (ft_strcmp(op, ">>") == 0)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

void	handle_out(char **args, t_data *p, int *i, int *fd)
{
	if (!args[*i + 1])
		redir_error_exit("Syntax error near >", p);
	*fd = open_file_for_output(args[*i], args[*i + 1]);
	if (*fd < 0)
		redir_error_exit("Open output failed", p);
	if (dup2(*fd, STDOUT_FILENO) < 0)
		redir_error_exit("dup2 output failed", p);
	close(*fd);
	remove_redir_tokens(p, *i);
}

void	handle_in(char **args, t_data *p, int *i, int *fd)
{
	if (!args[*i + 1])
		redir_error_exit("Syntax error near <", p);
	*fd = open(args[*i + 1], O_RDONLY);
	if (*fd < 0)
		redir_error_exit("Open input failed", p);
	if (dup2(*fd, STDIN_FILENO) < 0)
		redir_error_exit("dup2 input failed", p);
	close(*fd);
	remove_redir_tokens(p, *i);
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
			rd_isheredoc(*head, cmd_args, i);
		else
			(*i)++;
	}
	return (-100);
}
