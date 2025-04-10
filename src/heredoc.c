/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:25:59 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/10 19:35:58 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void rd_isheredoc(t_export *head, char **cmd_args, int *i)
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
void heredoc_parent(int pipefd[2], char *delimiter, char **cmd_args, int *i)
{
	int status;
	
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
void heredoc_child(t_export *head, char *delimiter, int pipefd[2])
{
	char *line;
	char *expanded;

	close(pipefd[0]);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\n')
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
void error_fork_heredoc(char **cmd_args, char *delimiter, int *pipefd)
{
	perror("couldnt fork in heredoc");
	free(delimiter);
	close(pipefd[0]);
	close(pipefd[1]);
	free_2d_array(cmd_args);
	return ;
}
void error_pipe_heredoc(char **cmd_args, char *delimiter)
{
	perror("couldnt create pipe in heredoc");
	free(delimiter);
	free_2d_array(cmd_args);
	return ;
}
int check_input_heredoc(char **cmd_args, int *i)
{
	if (!cmd_args[*i + 1])
	{
		printf("error no delimiter given\n");
		free_2d_array(cmd_args);
		return 0;
	}
	return 1;
}