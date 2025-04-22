/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:25:59 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/22 18:51:03 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_forheredoc(char **cmd_args, int *i)
{
	free(cmd_args[*i]);
	free(cmd_args[*i + 1]);
	return ;
}

void	handle_all_heredocs(t_export *head, char **cmd_args)
{
	int		i;
	int		pipefd[2];
	pid_t	pid;

	i = 0;
	if (pipe(pipefd) < 0)
		return ;
	while (cmd_args[i])
	{
		if (!ft_strcmp(cmd_args[i], "<<") && cmd_args[i + 1])
		{
			pid = fork();
			if (pid < 0)
				return ;
			if (pid == 0)
				child_write_heredoc(head, cmd_args[i + 1], pipefd[1]);
			heredoc_utils(pid, cmd_args, &i);
			continue ;
		}
		i++;
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], 0) < 0)
		return ;
	close(pipefd[0]);
}

void	heredoc_utils(pid_t pid, char **cmd_args, int *i)
{
	waitpid(pid, NULL, 0);
	free_forheredoc(cmd_args, i);
	shift_tokens(cmd_args, i, 2);
}

void	child_write_heredoc(t_export *head, char *delim, int fd)
{
	char	*line;
	char	*token;

	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (line_matches_delim(line, delim))
		{
			free(line);
			break ;
		}
		token = expand_single_token(line, head);
		write(fd, token, ft_strlen(token));
		free(token);
	}
	close(fd);
	exit(0);
}

int	line_matches_delim(char *line, char *delim)
{
	size_t	delim_len;
	size_t	line_len;

	delim_len = ft_strlen(delim);
	line_len = ft_strlen(line);
	if (line_len == delim_len + 1)
	{
		if (!ft_strncmp(line, delim, delim_len) && line[delim_len] == '\n')
			return (1);
	}
	return (0);
}
