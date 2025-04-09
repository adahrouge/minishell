/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:44:51 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/09 18:40:16 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	input_contains_redirection(t_data *p)
{
	char	**cmd_args;
	int		i;

	cmd_args = split_cmd_quoted(p->input);
	if (!cmd_args)
		return (0);
	i = 0;
	while (cmd_args[i] != NULL)
	{
		if (ft_strcmp(cmd_args[i], ">") == 0 || ft_strcmp(cmd_args[i], "<") == 0
			|| ft_strcmp(cmd_args[i], ">>") == 0
			|| ft_strcmp(cmd_args[i], "<<") == 0)
		{
			free_2d_array(cmd_args);
			return (1);
		}
		i++;
	}
	free_2d_array(cmd_args);
	return (0);
}

int	rd_input_correct(t_data *p)
{
	char	**cmd_args;
	int		i;

	cmd_args = split_cmd_quoted(p->input);
	if (!cmd_args)
		return (0);
	i = 0;
	while (cmd_args[i] != NULL)
	{
		if (strcmp(cmd_args[i], ">") == 0 || strcmp(cmd_args[i], "<") == 0
			|| strcmp(cmd_args[i], ">>") == 0 || strcmp(cmd_args[i], "<<") == 0)
		{
			if (cmd_args[i + 1] == NULL)
			{
				free_2d_array(cmd_args);
				return (0);
			}
			if (!next_arg_rd(cmd_args, &i))
				return (0);
		}
		i++;
	}
	free_2d_array(cmd_args);
	return (1);
}

int	next_arg_rd(char **cmd_args, int *i)
{
	if (strcmp(cmd_args[*i + 1], ">") == 0
		|| strcmp(cmd_args[*i + 1], "<") == 0
		|| strcmp(cmd_args[*i + 1], ">>") == 0
		|| strcmp(cmd_args[*i + 1], "<<") == 0)
	{
		printf("error next token is rd");
		return (0);
	}
	return (1);
}
char **parse_rd(t_data *p)
{
	char **cmd_args;
	cmd_args = split_cmd_quoted(p->input);
	if (!cmd_args)
		return NULL;
	return cmd_args;
}
