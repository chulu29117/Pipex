/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_for_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:49:49 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 23:05:00 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

static void	check_quote(char *quote, char c)
{
	if (*quote == 0 && (c == '\'' || c == '\"'))
		*quote = c;
	else if (*quote == c)
		*quote = 0;
}

int	count_words(char *cmd)
{
	int		i;
	int		count;
	int		in_word;
	char	quote;

	i = 0;
	count = 0;
	in_word = 0;
	quote = 0;
	while (cmd[i])
	{
		check_quote(&quote, cmd[i]);
		if (!is_whitespace(cmd[i]) || quote != 0)
		{
			if (in_word == 0)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		i++;
	}
	return (count);
}

static void	skip_quotes(char *cmd, int *i, char *quote, int *start)
{
	*quote = 0;
	if (cmd[*i] == '\'' || cmd[*i] == '\"')
	{
		*quote = cmd[*i];
		(*start)++;
		(*i)++;
	}
}

char	*extract_str(char *cmd, int *i)
{
	int		start;
	int		len;
	char	quote;
	char	*substr;

	start = *i;
	len = 0;
	skip_quotes(cmd, i, &quote, &start);
	while (cmd[*i] && (quote != 0 || !is_whitespace(cmd[*i])))
	{
		if (quote != 0 && cmd[*i] == quote)
			break ;
		(*i)++;
		len++;
	}
	if (quote != 0 && cmd[*i] == quote)
		(*i)++;
	substr = ft_substr(cmd, start, len);
	if (!substr)
		pipex_error("pipex: ft_substr failed");
	return (substr);
}
