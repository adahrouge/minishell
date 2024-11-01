/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 08:43:23 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/01 08:57:25 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int input_is_null(t_data *p)
{
	if (p->input == NULL)
	{
		  write(1, "exit\n", 5);
		  free(p->input);
			return 1;
	}
	return 0;
}
int input_is_backslash(t_data *p)
{
	if (p->input[0] == '\0')
    {
    	free(p->input);
		return 1;
	}
	return 0;
}
int input_is_exit(t_data *p)
{
	if (ft_strcmp(p->input, "exit") == 0)
	{
		free(p->input);
		return 1;
	}
	return 0;
}