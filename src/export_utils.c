/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 08:33:30 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/04 08:33:48 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void print_exported_variables(t_export *tmp, int i)
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