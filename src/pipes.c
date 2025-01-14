/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:19:11 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/14 13:07:18 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char **parse_pipes(t_data *p)
{
	p->cmds_pipe = ft_split(p->input, '|');
	if (!p->cmds_pipe)
		return NULL;
	return p->cmds_pipe;
}
int **create_pipes(t_data *p, int num_commands)
{
	int i;

	p->pipefd = malloc(sizeof(int *) * (num_commands - 1)); // have tofree
	if (!p->pipefd)
		return NULL;
	i = 0;
	while (i < num_commands - 1)
	{
		p->pipefd[i] = malloc(2 * sizeof(int)); // have to free
		if (!p->pipefd[i])
		{
			free_pipe(p, i);
			return NULL;
		}
		if (pipe(p->pipefd[i]) == -1)
		{
			perror("piping didnt work");
			free_pipe(p, i);
			return NULL;
		}
		i++;
	}
	return p->pipefd;
}
void first_command(t_data *p, int *i)
{
	dup2(p->pipefd[*i][1], STDOUT_FILENO); // STDOUT Becomes the write end of the pipe
	close(p->pipefd[*i][0]);
	close(p->pipefd[*i][1]);
}

void middle_commands(t_data *p, int *i)
{
	dup2(p->pipefd[*i - 1][0], STDIN_FILENO);
	dup2(p->pipefd[*i][1], STDOUT_FILENO);
	close(p->pipefd[*i - 1][0]);
	close(p->pipefd[*i - 1][1]);
	close(p->pipefd[*i][0]);
	close(p->pipefd[*i][1]);
}
void last_command(t_data *p, int *i)
{
	dup2(p->pipefd[*i - 1][0], STDIN_FILENO);
	close(p->pipefd[*i - 1][0]);
	close(p->pipefd[*i - 1][1]);
}
void create_fork(t_data *p, int num_commands, t_export *head)
{
	int i;
	pid_t pid;

	i = 0;
	while (i < num_commands)
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0) 
		{
			if (i == 0)
				first_command(p, &i);
			else if (i == num_commands - 1)
				last_command(p, &i);
			else
				middle_commands(p, &i);
			execute_command_pipes(p, &i, head);
		}
		else
		if (i > 0)
		{
			close(p->pipefd[i - 1][0]);
			close(p->pipefd[i - 1][1]);
		}
		i++;
	}
}
void execute_command_pipes(t_data *p, int *i, t_export *head)
{
	char **args;

	char **envp;
	envp = convert_list_to_array(head);
	if (!envp)
	{
		perror("failed to convert from list to array");
		exit(EXIT_FAILURE);
	}
	args = ft_split(p->cmds_pipe[*i], ' ');
	if (!args)
	{
		perror("Failed to split command");
		free_split(envp);
		exit(EXIT_FAILURE);
	}
	execve(args[0], args, envp);
	perror("execve failed");
	free_2d_array(envp);
	free_2d_array(args);
	exit(EXIT_FAILURE);
}

char **convert_list_to_array(t_export *head)
{
	int i;
	char **envp;
	t_export *ptr;

	ptr = head;
	i = 0;
	while (ptr != NULL) // do a function for the length for norminette
	{
		ptr = ptr->next;
		i++;
	}
	envp = malloc((i + 1) * sizeof(char *)); 
	if (!envp)
		return NULL;
	ptr = head;
	i = 0;
	while (ptr != NULL)
	{
		envp[i] = malloc((ft_strlen(ptr->data) + 1) * sizeof(char));
		if (!envp[i])
		{
			free_2d_array(envp);
			return NULL;
		}
		ft_strcpy(envp[i], ptr->data);
		ptr = ptr->next;
		i++;
	}
	envp[i] = NULL;
	return envp;
}

void minishell_pipes(t_data *p, t_export *head)
{
	int num_commands;
	num_commands = 0;
	p->cmds_pipe = parse_pipes(p);
	while (p->cmds_pipe[num_commands] != NULL)
		num_commands++;
	
	p->pipefd = create_pipes(p, num_commands);
	create_fork(p, num_commands, head);
	free_pipe(p, num_commands);
}




// Step-by-Step Flow with an Example
// Input:

// ls | grep ".txt"
// Parent Process Creates a Pipe:
// pipe(pipefd) creates a pipe:
// pipefd[0]: The read end.
// pipefd[1]: The write end.
// First Child (for ls):
// Goal: Redirect its output to the pipe's write end.
// Call: dup2(pipefd[1], STDOUT_FILENO)
// Result: Anything ls prints will go into the pipe instead of the terminal.
// Close Unused Ends:
// Close pipefd[0] because ls doesn’t read from the pipe.
// Close pipefd[1] after redirection to prevent resource leaks.
// Second Child (for grep ".txt"):
// Goal: Redirect its input to the pipe’s read end.
// Call: dup2(pipefd[0], STDIN_FILENO)
// Result: grep will take its input from the pipe (which contains the output of ls).