/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 20:24:18 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/09 19:20:19 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redirections(t_data *p, t_export **head)
{
	char **cmd_args;
	pid_t pid;
	int status;

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
void handle_parent_rd(t_data *p, pid_t pid, char **cmd_args, int status)
{
	waitpid(pid, &status, 0);
	free_2d_array(cmd_args);
	if (WIFEXITED(status))
		p->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		p->exit_code = 128 + WTERMSIG(status);
	return ;
}
void handle_child_rd(t_data *p, t_export **head, char **cmd_args)
{
	int i;
	int fd;

	i = 0;
	fd = 0;
	p->cmd_args = cmd_args;
	while (cmd_args[i])
	{
		if (ft_strcmp(cmd_args[i], ">") == 0)
			rd_isoutput(cmd_args, &fd, &i);
		else if (ft_strcmp(cmd_args[i], ">>") == 0)
			rd_isappend(cmd_args, &fd, &i);
		else if (ft_strcmp(cmd_args[i], "<") == 0)
			rd_isinput(cmd_args, &fd, &i);
		else
			i++;
	}
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

void execute_commands_rd(char *full_path, char **execve_args, char **envp, t_data *p)
{
	pid_t pid;
	int status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		execve(full_path, execve_args, envp);
		perror("execve failed in rd");
		free(full_path);
		free_2d_array(envp);
		free_2d_array(execve_args);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		parent_execution(pid, status, p, full_path);
		free_2d_array(execve_args);
		free_2d_array(envp);
	}
	else
	{
		free(full_path);
		free_2d_array(envp);
	}
}
void external_commands_rd(t_data *p, t_export **head, char **cmd_args)
{
	char **envp;
	int i;
	char **execve_args;
	char *path_env;
	char *full_path;

	path_env = NULL;
	i = 0;
	envp = convert_list_to_array(*head);
	execve_args = create_execve_rd(cmd_args);
	while (cmd_args[i] != NULL)
		i++;
	full_path = create_path_rd(p, *head, path_env);
	if (full_path)
		execute_commands_rd(full_path, execve_args, envp, p);
	return ;
	
}
char *create_path_rd(t_data *p, t_export *head, char *path_env)
{
	char **paths;
	char **new_paths;
	char *full_path;

	path_env = my_getenv("PATH", head);
	if (!path_env)
		return NULL;
	paths = ft_split(path_env, ':');
	new_paths = create_new_path(paths);
	if (!new_paths)
	{
		free_2d_array(paths);
		return NULL;
	}
	copy_paths(paths, new_paths);
	full_path = create_full_path(p, new_paths);
	free_2d_array(paths);
	free_2d_array(new_paths);
	return full_path;
}

char **create_execve_rd(char **cmd_args)
{
	char **execve_args;
	int i;
	int j;

	j = 0;
	i = 0;
	while (cmd_args[i] != NULL
		&&ft_strcmp(cmd_args[i], ">") != 0
		&& ft_strcmp(cmd_args[i], ">>") != 0
		&& ft_strcmp(cmd_args[i], "<") != 0)
		i++;
	execve_args = malloc((i + 1) * sizeof(char *));
	if (!execve_args)
		return NULL;
	while (j < i)
	{
		execve_args[j] = ft_strdup(cmd_args[j]);
		if (!execve_args[j])
		{
			free_my_environ(execve_args, j);
			return NULL;
		}
		j++;
	}
	execve_args[j] = NULL;
	return execve_args;
}

void rd_isinput(char **cmd_args, int *fd, int *i)
{
	 if (!cmd_args[*i + 1])
	 {
		 fprintf(stderr, "Syntax error: '<' requires a filename\n");
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

void rd_isappend(char **cmd_args, int *fd, int *i)
{
	if (!cmd_args[*i + 1])
	{
		fprintf(stderr, "Syntax error: '>>' requires a filename\n");
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
void rd_isoutput(char **cmd_args, int *fd, int *i)
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
void shift_tokens(char **cmd_args, int *i, int offset)
{
	int j;

	j = *i;
	while (cmd_args[j + offset])
	{
		cmd_args[j] = cmd_args[j + offset];
		j++;
	}
	cmd_args[j] = NULL;
}
