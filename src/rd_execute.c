/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:39:00 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/14 17:29:00 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands_rd(char *full_path, char **execve_args,
	char **envp, t_data *p)
{
	pid_t	pid;
	int		status;

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

void	external_commands_rd(t_data *p, t_export **head, char **cmd_args)
{
	char	**envp;
	int		i;
	char	**execve_args;
	char	*path_env;
	char	*full_path;

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

char	*create_path_rd(t_data *p, t_export *head, char *path_env)
{
	char	**paths;
	char	**new_paths;
	char	*full_path;

	path_env = my_getenv("PATH", head);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	new_paths = create_new_path(paths);
	if (!new_paths)
	{
		free_2d_array(paths);
		return (NULL);
	}
	copy_paths(paths, new_paths);
	full_path = create_full_path(p, new_paths);
	free_2d_array(paths);
	free_2d_array(new_paths);
	return (full_path);
}

char	**create_execve_rd(char **cmd_args)
{
	char	**execve_args;
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (cmd_args[i] != NULL && ft_strcmp(cmd_args[i], ">") != 0
		&& ft_strcmp(cmd_args[i], ">>") != 0
		&& ft_strcmp(cmd_args[i], "<") != 0)
		i++;
	execve_args = malloc((i + 1) * sizeof(char *));
	if (!execve_args)
		return (NULL);
	while (j < i)
	{
		execve_args[j] = ft_strdup(cmd_args[j]);
		if (!execve_args[j])
		{
			free_my_environ(execve_args, j);
			return (NULL);
		}
		j++;
	}
	execve_args[j] = NULL;
	return (execve_args);
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
