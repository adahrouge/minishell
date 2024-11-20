/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:45:57 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/20 20:22:45 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

t_export *populate_list(t_export *head)
{
	int i;
	t_export *ptr;
	t_export *newnode;
	
	i = 1;
	head->data = malloc(ft_strlen(environ[0]) + 1);
	if (!head->data)
		return NULL;
	ft_strcpy(head->data, environ[0]);
	head->next = NULL;
	ptr = head;
	while(environ[i] != NULL)
	{
		newnode = malloc(sizeof(t_export));
		if (!newnode)
			exit(1);
		newnode->data = malloc(ft_strlen(environ[i]) + 1);
		if (!newnode->data)
			return NULL;
		ft_strcpy(newnode->data, environ[i]);
		newnode->next = NULL;
		ptr->next = newnode;
		ptr = newnode;
		i++;
	}
	return head;
}


int env(void)
{
	int i;
	int j;
	i = 0;
	while (environ[i] != NULL)
	{
		j = 0;
		while (environ[i][j] != '\0')
		{
			write(1, &environ[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
	return 1;
}
int unset(char *str)
{
    int i;
	i = 0;
    if (getenv(str) == NULL)
    {
        printf("Error, environment variable not found\n");
        return 0;
    }
    while (environ[i] != NULL)
    {
        if (ft_strncmp(environ[i], str, ft_strlen(str)) == 0 && environ[i][ft_strlen(str)] == '=')
        {
            while (environ[i] != NULL)
            {
                environ[i] = environ[i + 1];
                i++;
            }
            return 1;
        }
        i++;
    }
    return 0;
}
int export(void)
{
    t_export *head;
    t_export *tmp;
	int i;

	i = 0;
    head = malloc(sizeof(t_export));
    	if (!head)
        	return 0;

	head = populate_list(head);
	head = sort_list(head);
	tmp = head;
	print_variables(tmp, i);
	free_list(head);
	return 0;
}

int export_main(t_data *p)
{
	int i;
	t_export *head;
	head = malloc(sizeof(t_export));
	if (!head)
		return 0;
	head = populate_list(head);
	head = sort_list(head);
	arrange_export(head);
	i = 1;
	if (ft_strcmp(p->input, "export") == 0 && p->cmd_args[1] == NULL)
		export();
	else
	{
		while (p->cmd_args[i] != NULL)
		{
			process_export_args(p->cmd_args[i], &head);
			i++;
		}
		free_list_aft_export(head);
	}
	return 1;
}
void process_export_args(char *arg, t_export **head)
{
	int i;
	int flag;
	
	flag = 0;
	i = 0;
	if (!(check_arg_export(arg)))
		return ;
	while (arg[i] != '\0')
	{
		if (arg[i] == '=')
		flag = 1;
	i++;
	}
	if (flag == 0)
		process_export_empty_arg(arg, head);
	else
		process_export_full_arg(arg, head);
}
int process_export_full_arg(char *arg, t_export **head)
{
	int i;
	int equal_position;
	char *var_name;
	char *var_value;

	i = 0;
	equal_position = ft_strchr(arg, '=');
	if (equal_position == -1)
	{
		printf("error, no equal position while trying to divide export between NAME AND VALUE");
		return 0;
	}
	var_name = ft_substr(arg, 0, equal_position);
	var_value = ft_substr(arg, equal_position + 1,
		ft_strlen(arg) - equal_position - 1);
	if (!var_name || !var_value)
		return 0;
	update_arg_in_list(var_name, var_value, head);
	free(var_name);
	free(var_value);
	return 1;
}
int update_arg_in_list(char *var_name, char *var_value, t_export **head_ref)
{
	t_export *temp;
	t_export *newnode;
	t_export *prev;

	prev = NULL;
	temp = *head_ref;
	while (temp != NULL)
	{
		if (ft_strcmp(temp->name, var_name) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(var_value);
			return 1;
		}
		prev = temp;
		temp = temp->next;
	}
	newnode = create_node(var_name, var_value);
	if (prev != NULL)
		prev->next = newnode;
	else
	*head_ref = newnode;
	return 1;
}
void free_new_node(t_export *newnode)
{
		free(newnode->name);
		free(newnode->value);
		free(newnode);
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
	return newnode;
}
int arrange_export(t_export *head)
{
	t_export *tmp;
	int equal_pos;

	tmp = head;
	while (tmp != NULL)
	{
		equal_pos = ft_strchr(tmp->data, '=');
		if (equal_pos == -1)
			{
				tmp->name = ft_strdup(tmp->data);
				tmp->value = NULL;
			}
		tmp->name = ft_substr(tmp->data, 0, equal_pos);
		tmp->value = ft_substr(tmp->data, equal_pos + 1,
			ft_strlen(tmp->data) - equal_pos - 1);
		tmp = tmp->next;
	}
	return 1;
}

int check_arg_export(char *arg)
{
	int i;
	i = 1;
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
	{
		printf("error, invalid identifier");
		return -1;
	}
	while (arg[i] != '\0' && arg[i] != '=')
	{
		if (!(ft_isnum(arg[i]) || arg[i] == '_'))
		{
			printf("export : '%s\n' : not a valid identifier", arg);
			return -1;
		}
		i++;
	}
	return 1;
}

void free_list_aft_export(t_export *head)
{
	t_export *temp;
	
	while (head != NULL)
	{
		temp = head;
		free(head->name);
		free(head->value);
		free(head->data);
		head = head->next;
		free(temp);
	}
}


