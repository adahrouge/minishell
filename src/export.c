/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:45:57 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/01 14:17:11 by adahroug         ###   ########.fr       */
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

t_export *sort_list(t_export *head)
{
	t_export *slow;
	t_export *fast;
	t_export *head2;
	t_export *left;
	t_export *right;
	
	slow = head;
	fast = head->next;
	if (head == NULL || head->next == NULL)
		return head;
	while (fast != NULL && fast->next != NULL)
	{
		fast = fast->next->next;
		slow = slow->next;
	}
	head2 = slow->next;
	slow->next = NULL;
	left = sort_list(head);
	right = sort_list(head2);
	return merged_sorted_list(left, right);
}
t_export *merged_sorted_list(t_export *head, t_export *head2)
{
	if (head == NULL)
	return head2;
	if (head2 == NULL)
	return head;
	if (ft_strcmp(head->data, head2->data) < 0)
	{
		head->next = merged_sorted_list(head->next, head2);
		return head;
	}
	else 
	{
		head2->next = merged_sorted_list(head, head2->next);
		return head2;
	}
}
void free_list(t_export *head)
{
	t_export *temp;
	while (head != NULL)
	{
		temp = head;
		free(head->data);
		head = head->next;
		free(temp);
	}
}
int export(void)
{
	t_export *head;
	t_export *tmp;

	head = malloc(sizeof(t_export));
	if (!head)
		return 0;
	head = populate_list(head);
	head = sort_list(head);
	tmp = head;
	while (tmp != NULL)
	{
		printf("declare -x %s\n" , tmp->data); // handle double quotation after ""
		tmp= tmp->next;
	}
	free_list(head);
	return 0;
}