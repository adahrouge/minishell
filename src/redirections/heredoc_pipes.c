/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:01:42 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/14 16:59:38 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_pipes(t_data *p, t_export *head, int *i, int fd)
{
	char	*dup_delim;

	if (!p->cmd_args[*i + 1])
		perror("syntax error near <<");
	dup_delim = ft_strdup(p->cmd_args[*i + 1]);
	fd = create_heredoc_fd(head, dup_delim);
	if (dup2(fd, STDIN_FILENO) < 0)
		perror("dup2 heredoc failed");
	close(fd);
	remove_redir_tokens(p, *i);
}

int	create_heredoc_fd(t_export *head, char *delim)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	status = 0;
	if (pipe(pipefd) < 0)
		perror("pipe heredoc failed");
	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		perror("fork failed pipe heredoc");
	}
	if (pid == 0)
		heredoc_child(head, delim, pipefd);
	else
		return (heredoc_parent_pipe(delim, pipefd, &status));
	return (-1);
}

int	heredoc_parent_pipe(char *delim, int pipefd[2], int *status)
{
	int	wstatus;

	close(pipefd[1]);
	waitpid(-1, &wstatus, 0);
	if (WIFEXITED(wstatus))
		*status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		*status = 128 + WTERMSIG(wstatus);
	free(delim);
	return (pipefd[0]);
}
