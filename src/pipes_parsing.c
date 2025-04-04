/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:22:29 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/19 12:03:25 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	parse_pipe_arg(t_data *p)
{
	int	i;
	int	in_quotes;

	p->store_pipe_arg = NULL;
	i = 0;
	in_quotes = 0;
	p->nb_of_pipes = 0;
	while (p->input[i] != '\0')
	{
		if (in_quotes == 0 && (p->input[i] == '\'' || p->input[i] == '"'))
			in_quotes = 1;
		else if (in_quotes == 1 && (p->input[i] == '\'' || p->input[i] == '"'))
			in_quotes = 0;
		else if (in_quotes == 0 && p->input[i] == '|')
			p->nb_of_pipes++;
		i++;
	}
	p->store_pipe_arg = malloc((p->nb_of_pipes + 2) * sizeof(char *));
	if (!p->store_pipe_arg)
	{
		perror("failed malloc store_pipe_arg");
		exit(EXIT_FAILURE);
	}
}

void	handle_quotes_pipes(char c, int *in_quotes)
{
	if (!*in_quotes && (c == '\'' || c == '"'))
		*in_quotes = 1;
	else if ((*in_quotes == 1) && (c == '\'' || c == '"'))
		*in_quotes = 0;
}

void	handle_pipe(t_data *p, int *len, int *count, int *i)
{
	if (*len > 0)
	{
		create_single_arg(p, len, count, *i - *len);
		(*count)++;
	}
	(*i)++;
	*len = 0;
	while (p->input[*i] == ' ' || p->input[*i] == '\t')
		(*i)++;
}

void	trim_whitespaces(char *str)
{
	int	i;
	int	start;
	int	end;

	if (str == NULL || str[0] == '\0')
		return ;
	i = 0;
	start = 0;
	end = ft_strlen(str) - 1;
	while (str[start] != '\0' && ((str[start] == ' ')
			|| (str[start] >= 9 && str[start] <= 13)))
		start++;
	while (end > start && (str[end] == ' '
			|| (str[end] >= 9 && str[end] <= 13)))
		end--;
	while (start <= end)
	{
		str[i] = str[start];
		i++;
		start++;
	}
	str[i] = '\0';
}

void	parse_pipes(t_data *p)
{
	int	i;

	i = 0;
	parse_pipe_arg(p);
	create_pipe_arg(p, i);
	trim_pipe_args(p);
	p->num_commands = p->nb_of_pipes + 1;
}
