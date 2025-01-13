/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_argument.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 15:39:49 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/11 13:12:51 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int process_export_args(char *arg, t_export **head)
{
	int i;
	int flag;
	
	flag = 0;
	i = 0;
	if (!(check_arg_export(arg)))
		return 1;
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
	return 0;
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
			if (!modify_value(temp, var_value))
				return 0;
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
