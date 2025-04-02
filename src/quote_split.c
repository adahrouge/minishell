/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:45:24 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/01 18:08:41 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_quotes(const char *str, int *i)
{
	char	quote_char;

	quote_char = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote_char)
		(*i)++;
	if (str[*i])
		(*i)++;
}

int	count_tokens(const char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
			i++;
		if (!str[i])
			break ;
		count++;
		while (str[i] && !(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		{
			if (str[i] == '"' || str[i] == '\'')
				skip_quotes(str, &i);
			else
				i++;
		}
	}
	return (count);
}
char *copy_token(const char *str, int start, int end)
{
    char *token;
    int src;
    int dst;

    token = malloc(end - start + 1);
    if (!token)
        return NULL;
    src = start;
    dst = 0;
    while (src < end)
    {
        token[dst] = str[src];
        dst++;
        src++;
    }
    token[dst] = '\0';
    return token;
}

char	**split_cmd_quoted(const char *str)
{
	int		token_count;
	char	**tokens;

	token_count = count_tokens(str);
	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	tokens = split_cmd_quoted_core(str, tokens, token_count);
	tokens[token_count] = NULL;
	return (tokens);
}

char	**split_cmd_quoted_core(const char *str, char **tokens, int token_count)
{
	int	i;
	int	t;
	int	start;
	int	end;

	i = 0;
	t = 0;
	while (t < token_count)
	{
		while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
			i++;
		start = i;
		while (str[i] && !(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		{
			if (str[i] == '"' || str[i] == '\'')
				skip_quotes(str, &i);
			else
				i++;
		}
		end = i;
		tokens[t] = copy_token(str, start, end);
		t++;
	}
	return (tokens);
}
