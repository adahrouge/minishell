/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:22:29 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/20 21:45:38 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void parse_pipe_arg(t_data *p)
{
	int i;
	int in_quotes;

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
	p->store_pipe_arg = malloc((p->nb_of_pipes + 2) * sizeof(char *)); // one for null and one for the extra arg
	if (!p->store_pipe_arg) 
		{
			perror("failed malloc store_pipe_arg");
			exit(EXIT_FAILURE);
		}
}

void create_pipe_arg(t_data *p)
{
    int i = 0;
    int in_quotes = 0;
    int len = 0;        // how many chars we have in current segment
    int count = 0;      // index for p->store_pipe_arg

    while (p->input[i] != '\0')
    {
        // Track quotes
        if (!in_quotes && (p->input[i] == '\'' || p->input[i] == '"'))
            in_quotes = 1;
        else if (in_quotes && (p->input[i] == '\'' || p->input[i] == '"'))
            in_quotes = 0;
        // If we see a pipe outside quotes, that's the end of one segment
        if (!in_quotes && p->input[i] == '|')
        {
            // Create substring from [i - len, i) (exclude the pipe char itself)
            if (len > 0)
            {
                create_single_arg(p, &len, &count, i - len);
                count++;
            }
            // Skip the pipe char
            i++;
            len = 0;
        }
        else
        {
            len++;
            i++;
        }
    }
    // Handle the final chunk after the last pipe
    if (len > 0)
    {
        create_single_arg(p, &len, &count, i - len);
        count++;
    }

    p->store_pipe_arg[count] = NULL; 
}


void create_single_arg(t_data *p, int *len, int *count, int start)
{
    p->store_pipe_arg[*count] = malloc((*len + 1) * sizeof(char));
    if (!p->store_pipe_arg[*count])
    {
        perror("Failed to allocate memory for pipe segment");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *len; i++)
        p->store_pipe_arg[*count][i] = p->input[start + i];

    p->store_pipe_arg[*count][*len] = '\0';
}

void trim_whitespaces(char *str)
{
	int i;
	int start;
	int end;

	if (str == NULL || str[0] == '\0')
		return;
	i = 0;
	start = 0;
	end = ft_strlen(str) - 1;
	while (str[start] != '\0' && ((str[start] == ' ') || (str[start] >= 9 && str[start] <= 13)))
		start++;
	while (end > start && (str[end] == ' ' || (str[end] >= 9 && str[end] <= 13)))
		end--;
	while (start <= end)
		{
			str[i] = str[start];
			i++;
			start++;
		}
		str[i] = '\0';
}
void trim_pipe_args(t_data *p)
{
	int i;

	i = 0;
	while (p->store_pipe_arg[i] != NULL)
	{
		trim_whitespaces(p->store_pipe_arg[i]);
		remove_all_quotes_pipes(p->store_pipe_arg[i]);
		i++;
	}
}
void remove_all_quotes_pipes(char *str)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
}

