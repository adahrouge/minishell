/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_populate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 15:38:45 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/24 15:39:21 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_export *populate_list(char **environ)
{
	int i;
	t_export *head;
	t_export *newnode;
	t_export *ptr;

	head = NULL;
	ptr = NULL;
	i = 0;
	while (environ[i] != NULL)
	{
		newnode = create_my_list(environ, i);
		if (!newnode)
		{
			free_list(head);
			return NULL;
		}
		check_for_populate(&head, &ptr, newnode);
		i++;
	}
	return head;
}

t_export *create_my_list(char **environ, int i)
{
	t_export *newnode;

	 newnode = malloc(sizeof(t_export));
        if (!newnode)
            return NULL;
        newnode->data = malloc(ft_strlen(environ[i]) + 1);
        if (!newnode->data)
        {
            free(newnode);
            return NULL;
        }
        ft_strcpy(newnode->data, environ[i]);
        newnode->name = NULL;
        newnode->value = NULL;
        newnode->next = NULL;
	return newnode;
}
void check_for_populate(t_export **head, t_export **ptr, t_export *newnode)
{
	if (*head == NULL)
        {
            *head = newnode;
            *ptr = *head;
        }
        else
        {
            (*ptr)->next = newnode;
            *ptr = newnode;
        }
}
void free_list(t_export *head)
{
	t_export *temp;
	while (head != NULL)
	{
		temp = head;
		if (head->data)
			free(head->data);
		if (head->name)
			free(head->name);
		if (head->value)
			free(head->value);
		head = head->next;
		free(temp);
	}
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
