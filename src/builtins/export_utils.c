/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 08:33:30 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/16 16:15:38 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_variables(t_export *tmp, int i)
{
	while (tmp != NULL)
	{
		i = 0;
		printf("declare -x ");
		while (tmp->data[i] != '\0' && tmp->data[i] != '=')
		{
			printf("%c", tmp->data[i]);
			i++;
		}
		if (tmp->data[i] == '=')
		{
			printf("%c", tmp->data[i]);
			printf("\"");
			i++;
			while (tmp->data[i] != '\0')
			{
				printf("%c", tmp->data[i]);
				i++;
			}
			printf("\"");
		}
		printf("\n");
		tmp = tmp->next;
	}
}

t_export	*sort_list(t_export *head)
{
	t_export	*slow;
	t_export	*fast;
	t_export	*head2;
	t_export	*left;
	t_export	*right;

	slow = head;
	fast = head->next;
	if (head == NULL || head->next == NULL)
		return (head);
	while (fast != NULL && fast->next != NULL)
	{
		fast = fast->next->next;
		slow = slow->next;
	}
	head2 = slow->next;
	slow->next = NULL;
	left = sort_list(head);
	right = sort_list(head2);
	return (merged_sorted_list(left, right));
}

t_export	*merged_sorted_list(t_export *head, t_export *head2)
{
	if (head == NULL)
		return (head2);
	if (head2 == NULL)
		return (head);
	if (ft_strcmp(head->data, head2->data) < 0)
	{
		head->next = merged_sorted_list(head->next, head2);
		return (head);
	}
	else
	{
		head2->next = merged_sorted_list(head, head2->next);
		return (head2);
	}
}

int	check_arg_export(char *arg)
{
	int	i;

	i = 1;
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
	{
		printf("error, invalid identifier\n");
		return (-1);
	}
	while (arg[i] != '\0' && arg[i] != '=')
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
		{
			printf("export : '%s\n' : not a valid identifier", arg);
			return (-1);
		}
		i++;
	}
	return (1);
}

void	free_new_node(t_export *newnode)
{
	if (newnode)
	{
		if (newnode->name)
			free(newnode->name);
		if (newnode->value)
			free(newnode->value);
		if (newnode->data)
			free(newnode->data);
		free(newnode);
	}
}
