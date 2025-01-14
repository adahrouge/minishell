/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 10:10:30 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/14 11:57:05 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void external_commands(t_data *p, t_export *head)
{
	char *path_env;
	char **paths;
	char **new_paths;
	char *full_path;

	path_env = getenv("PATH");
	if (!path_env)
		return ;
	paths = ft_split(path_env, ':');
	new_paths = create_new_path(paths);
		if (!new_paths)
		{
			free_2d_array(paths);
			perror("error: failed to create new_paths");
			return ;
		}
	copy_paths(paths, new_paths);
	full_path = create_full_path(p, new_paths);
	if (full_path != NULL)
		execute_command(full_path, p, head);
		else
		{
			printf("bash: %s: command not found\n", p->cmd_args[0]);
			p->exit_code = 127;
		}
	free_2d_array(paths);
	free_2d_array(new_paths);
	return ;
}
void execute_command(char *full_path, t_data *p, t_export *head)
{
	pid_t pid;
	char **envp;
	int status;

	envp = convert_list_to_array(head);
	pid = fork();
	if (pid == 0)
	{
		execve(full_path, p->cmd_args, envp);
		perror("execve failed");
		free(full_path);
		free_2d_array(envp);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			p->exit_code = WEXITSTATUS(status);
		free(full_path);
	}
	else
		free(full_path);
	p->exit_code = 1;
	free_2d_array(envp);
}
char *create_full_path(t_data *p, char **new_paths)
{
	int i;
	char *full_path;

	i = 0;
	while (new_paths[i] != NULL)
	{
	full_path = malloc(((ft_strlen(p->cmd_args[0]) + ft_strlen(new_paths[i])) + 1) * sizeof(char));
	if (!full_path)
		return NULL;
	ft_strcpy(full_path, new_paths[i]);
	strcat(full_path, p->cmd_args[0]);
		if (access(full_path, X_OK) == 0)
			return full_path;
			else
			free(full_path);
		i++;
	}
	return NULL;
}

void copy_paths(char **paths, char **new_paths)
{
	int i;
	int j;

	j = 0;
	i = 0;
	while (new_paths[i] != NULL)
	{
		j = 0;
		while (paths[i][j] != '\0')
		{
			new_paths[i][j] = paths[i][j];
			j++;
		}
		new_paths[i][j] = '/';
		new_paths[i][j + 1] = '\0';
		i++;
	}
}
char **create_new_path(char **paths)
{
	int i;
	int len;
	char **new_paths;

	i = 0;
	len = 0;
	while (paths[i] != NULL)
		i++;
	new_paths = malloc((i + 1) * sizeof(char *)); // must free
	if (!new_paths)
		return NULL;
	i = 0;
	while (paths[i] != NULL)
	{
		len = ft_strlen(paths[i]);
		new_paths[i] = malloc((len + 2) * sizeof(char)); // for '/' and '\0'
		if (!new_paths[i])
		{
			free_already_allocated(new_paths, i);
			return NULL;
		}
		i++;
	}
	return new_paths;
}


