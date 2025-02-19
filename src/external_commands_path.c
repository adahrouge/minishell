/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_path.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:53:36 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/19 11:55:29 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_full_path(t_data *p, char **new_paths)
{
	int		i;
	char	*full_path;

	i = 0;
	while (new_paths[i] != NULL)
	{
		full_path = malloc(((ft_strlen(p->cmd_args[0])
						+ ft_strlen(new_paths[i])) + 1) * sizeof(char));
		if (!full_path)
			return (NULL);
		ft_strcpy(full_path, new_paths[i]);
		strcat(full_path, p->cmd_args[0]);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		else
			free(full_path);
		i++;
	}
	return (NULL);
}

void	copy_paths(char **paths, char **new_paths)
{
	int	i;
	int	j;

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

char	**create_new_path(char **paths)
{
	int		i;
	int		len;
	char	**new_paths;

	i = 0;
	len = 0;
	while (paths[i] != NULL)
		i++;
	new_paths = malloc((i + 1) * sizeof(char *));
	if (!new_paths)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		len = ft_strlen(paths[i]);
		new_paths[i] = malloc((len + 2) * sizeof(char));
		if (!new_paths[i])
		{
			free_already_allocated(new_paths, i);
			return (NULL);
		}
		i++;
	}
	new_paths[i] = NULL;
	return (new_paths);
}

char	**convert_list_to_array(t_export *head)
{
	int			i;
	char		**envp;
	t_export	*ptr;

	ptr = head;
	i = 0;
	i = size_list(head);
	envp = malloc((i + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (ptr != NULL)
	{
		envp[i] = malloc((ft_strlen(ptr->data) + 1) * sizeof(char));
		if (!envp[i])
		{
			free_2d_array(envp);
			return (NULL);
		}
		ft_strcpy(envp[i], ptr->data);
		ptr = ptr->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
