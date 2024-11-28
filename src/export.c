/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:45:57 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/28 12:19:47 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int env(t_export *head)
{
	t_export *tmp;

	tmp = head;
	while (tmp != NULL && tmp->value != NULL)
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
		printf("bash: unset: %s: not a valid identifier\n", str);
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

void export_main(t_data *p, t_export **head)
{
	int i;
	*head = sort_list(*head);
	i = 1;
	if (ft_strcmp(p->input, "export") == 0 && p->cmd_args[1] == NULL)
		export(*head);
	else
	{
		while (p->cmd_args[i] != NULL)
		{
			process_export_args(p->cmd_args[i], head);
			i++;
		}
	}
	return ;
}





