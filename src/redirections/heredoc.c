/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:25:59 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/15 19:00:25 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rd_isheredoc(t_export *head, char **cmd_args, int *i)
{
	char	*delimiter;
	int		pipefd[2];
	pid_t	pid;

	if (!check_input_heredoc(cmd_args, i))
		return ;
	delimiter = ft_strdup(cmd_args[*i + 1]);
	if (pipe(pipefd) < 0)
	{
		error_pipe_heredoc(cmd_args, delimiter);
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid < 0)
	{
		error_fork_heredoc(cmd_args, delimiter, pipefd);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		heredoc_child(head, delimiter, pipefd);
	else
		heredoc_parent(pipefd, delimiter, cmd_args, i);
}

void	heredoc_parent(int pipefd[2], char *delimiter,
		char **cmd_args, int *i)
{
	int	status;

	status = 0;
	close(pipefd[1]);
	waitpid(-1, &status, 0);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(pipefd[0]);
		free(delimiter);
		free_2d_array(cmd_args);
		exit(1);
	}
	close(pipefd[0]);
	free(delimiter);
	free(cmd_args[*i]);
	free(cmd_args[*i + 1]);
	shift_tokens(cmd_args, i, 2);
}
int line_matches_delim(char *line, char *delim)
{
	size_t delim_len;
	size_t line_len;

	delim_len = ft_strlen(delim);
	line_len = ft_strlen(line);
	if (line_len == delim_len + 1 && ft_strncmp(line, delim, delim_len) == 0
		&& line[delim_len] == '\n')
		return 1;
	return 0;
}

void	heredoc_child(t_export *head, char *delimiter, int pipefd[2])
{
	char	*line;
	char	*expanded;

	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	close(pipefd[0]);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (line_matches_delim(line, delimiter))
		{
			free(line);
			break;
		}
		expanded = expand_single_token(line, head);
		write(pipefd[1], expanded, ft_strlen(expanded));
		free(expanded);
	}
	close(pipefd[1]);
	free(delimiter);
	exit(0);
}

int	check_input_heredoc(char **cmd_args, int *i)
{
	if (!cmd_args[*i + 1])
	{
		printf("error no delimiter given\n");
		free_2d_array(cmd_args);
		return (0);
	}
	return (1);
}
