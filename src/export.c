/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:45:57 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/22 18:34:23 by adahroug         ###   ########.fr       */
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
// t_export *populate_list(t_export *head, char **environ)
// {
// 	int i;
// 	t_export *ptr;
// 	t_export *newnode;
	
// 	i = 1;
// 	newnode = NULL;
// 	head->data = malloc(ft_strlen(environ[0]) + 1);
// 	if (!head->data)
// 	{
// 		free(head);
// 		return NULL;
// 	}
// 	ft_strcpy(head->data, environ[0]);
// 	head->next = NULL;
// 	ptr = head;
// 	while(environ[i] != NULL)
// 	{
// 		newnode = malloc(sizeof(t_export));
// 		if (!newnode)
// 		{
// 			free(newnode);
// 			return NULL;
// 		}
// 		newnode->data = malloc(ft_strlen(environ[i]) + 1);
// 		if (!newnode->data)
// 		{
// 			free(newnode);
// 			free_list(head);
// 			return NULL;
// 		}
// 		newnode->name = NULL;
// 		newnode->value = NULL;
// 		ft_strcpy(newnode->data, environ[i]);
// 		newnode->next = NULL;
// 		ptr->next = newnode;
// 		ptr = newnode;
// 		i++;
// 	}
// 	return head;
// }
t_export *populate_list(char **environ)
{
    int i = 0;
    t_export *head = NULL;
    t_export *ptr = NULL;
    t_export *newnode = NULL;

    while (environ[i] != NULL)
    {
        newnode = malloc(sizeof(t_export));
        if (!newnode)
        {
            free_list(head);
            return NULL;
        }
        newnode->data = malloc(ft_strlen(environ[i]) + 1);
        if (!newnode->data)
        {
            free(newnode);
            free_list(head);
            return NULL;
        }
        ft_strcpy(newnode->data, environ[i]);
        newnode->name = NULL;
        newnode->value = NULL;
        newnode->next = NULL;

        if (head == NULL)
        {
            head = newnode;
            ptr = head;
        }
        else
        {
            ptr->next = newnode;
            ptr = newnode;
        }
        i++;
    }
    return head;
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
		else
		{
			tmp->name = ft_substr(tmp->data, 0, equal_pos);
			tmp->value = ft_substr(tmp->data, equal_pos + 1,
			ft_strlen(tmp->data) - equal_pos - 1);
		}
		tmp = tmp->next;
	}
	return 1;
}

int env(t_export *head)
{
	t_export *tmp;

	tmp = head;
	while (tmp != NULL)
	{
	printf("%s\n", tmp->data);
	tmp = tmp->next;
	}
	return 1;
}
char *my_getenv(char *name, t_export *head)
{
	t_export *tmp;
	tmp = head;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, name) == 0)
			return tmp->value;
		tmp = tmp->next;
	}
	return NULL;
}
int unset(char *str, t_export **head)
{
	t_export *tmp;
	t_export *prev;

	tmp = *head;
	prev = NULL;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, str) == 0)
		{
			if (prev == NULL)
			*head = tmp->next;
			else
			prev->next = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp->data);
			free(tmp);
			return 1;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	printf("Error, environment variable not found\n");
	return 0;
}
int export(t_export *head)
{
    t_export *tmp;
	int i;

	i = 0;
	tmp = head;
	print_variables(tmp, i);
	return 0;
}

int export_main(t_data *p, t_export *head)
{
	int i;
	head = sort_list(head);
	i = 1;
	if (ft_strcmp(p->input, "export") == 0 && p->cmd_args[1] == NULL)
		export(head);
	else
	{
		while (p->cmd_args[i] != NULL)
		{
			process_export_args(p->cmd_args[i], &head);
			i++;
		}
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
void process_export_empty_arg(char *arg, t_export **head)
{
	t_export *tmp;
	t_export *prev;
	t_export *new;

	prev = NULL;
	tmp = *head;
	while (tmp != NULL)
	{
		if (ft_strcmp(arg, tmp->name) == 0)
			return ;
		prev = tmp;
		tmp = tmp->next;
	}
	new = create_empty_arg(arg);
	if (!new)
		return ;
	if (prev == NULL)
		*head = new;
	else
	prev->next = new;
}
t_export *create_empty_arg(char *arg)
{
	t_export *new;

	new = malloc(sizeof(t_export));
	if (!new)
		return NULL;
	new->name = malloc(ft_strlen(arg) + 1);
	if (!new->name)
		return NULL;
	new->data = malloc(ft_strlen(arg) + 1);
	if (!new->data)
		return NULL;
	ft_strcpy(new->name, arg);
	ft_strcpy(new->data, arg);
	new->value = NULL;
	new->next = NULL;
	return new;
}

int process_export_full_arg(char *arg, t_export **head)
{
	int equal_position;
	char *var_name;
	char *var_value;

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
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
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


