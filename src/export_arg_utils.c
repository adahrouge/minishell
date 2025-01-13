/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_arg_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 15:41:37 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/11 13:15:36 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char **copy_environ(char **environ)
{
	char **my_environ;
	int i;
	int count;

	count = 0;
	i = 0;
	while (environ[count] != NULL)
		count++;
	my_environ = malloc((count + 1) * sizeof(char *));
	if (!my_environ)
		return NULL;
	while (i < count)
	{
		my_environ[i] = malloc(ft_strlen(environ[i]) + 1);
		if (!my_environ[i])
			{
				free_my_environ(my_environ, i);
				return NULL;
			}
		ft_strcpy(my_environ[i], environ[i]);
		i++;
	}
	my_environ[count] = NULL;
	return my_environ;
}
int free_my_environ(char **my_environ, int i)
{
	int j;

	j = 0;
	while (j < i)
	{
		free(my_environ[j]);
		j++;
	}
	free(my_environ);
	return 1;
}
int ft_unset_all(char *input, t_export **head) 
{
    int i;
	int j;
    char arg[256];

	i = 0,
	j = 0;
    while (input[i] && input[i] != ' ')
        i++;
    while (input[i] == ' ') 
        i++;
    while (input[i]) 
	{
        j = 0;
        while (input[i] && input[i] != ' ') 
            arg[j++] = input[i++];
        arg[j] = '\0';
        unset(arg, head); 
        while (input[i] == ' ') 
            i++;
    }
    return 0;
}

int modify_value(t_export *temp, char *var_value)
{
    char *new_value;
	size_t data_len;
	char *new_data;

	new_value = ft_strdup(var_value);
    if (!new_value)
        return 0; 
    data_len = ft_strlen(temp->name) + ft_strlen(new_value) + 2;
    new_data = malloc(data_len);
    if (!new_data)
    {
        free(new_value);
        return 0; 
    }
    ft_strcpy(new_data, temp->name);
    strcat(new_data, "=");
    strcat(new_data, new_value);
    free(temp->value);
    free(temp->data);
    temp->value = new_value;
    temp->data = new_data;
    return 1; 
}

t_export *create_node(char *var_name, char *var_value)
{
	int i;
	t_export *newnode;
	char *var_name_modified;

	newnode = malloc(sizeof(t_export));
		if (!newnode)
		return NULL;
	newnode->name = ft_strdup(var_name);
	newnode->value = ft_strdup(var_value);
	if (!newnode->name || !newnode->value)
	{
		free_new_node(newnode);
		return NULL;
	}
	var_name_modified = malloc(ft_strlen(var_name) + 2);
	if (!var_name_modified)
			return NULL;
	ft_strcpy(var_name_modified, var_name);
	i = ft_strlen(var_name_modified);
	var_name_modified[i] = '=';
	var_name_modified[i + 1] = '\0';
	newnode->data = ft_strjoin(var_name_modified, var_value);
	free(var_name_modified);
	newnode->next = NULL;
	return newnode;
}