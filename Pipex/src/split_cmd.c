/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:33 by clu               #+#    #+#             */
/*   Updated: 2025/01/31 10:10:14 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	count_words(char *cmd)
{
	int		i;
	int		count;
	int		quotes;

	i = 0;
	count = 0;
	quotes = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			quotes = !quotes;
		if (!is_whitespace(cmd[i]) && !quotes)
		{
			if (i == 0 || is_whitespace(cmd[i - 1]))
				count++;
		}
		i++;
	}
	return (count);
}

static char	*get_str(char *cmd, int *i)
{
	int		start;
	int		len;
	char	quote;

	while (is_whitespace(cmd[*i]))
		(*i)++;
	start = *i;
	if (cmd[*i] == '\'' || cmd[*i] == '\"')
	{
		quote = cmd[*i];
		start++;
		(*i)++;
		while (cmd[*i] && cmd[*i] != quote)
			(*i)++;
		len = *i - start;
		if (cmd[*i])
			(*i)++;
	}
	else
	{
		while (cmd[*i] && !is_whitespace(cmd[*i]))
			(*i)++;
		len = *i - start;
	}
	return (ft_substr(cmd, start, len));
}

char	**split_cmd(char *cmd)
{
	char	**substr;
	int		i;
	int		word_count;
	int		word_index;
	char	*str;

	word_count = count_words(cmd);
	substr = malloc(sizeof(char *) * (word_count + 1));
	if (!substr)
		return (NULL);
	i = 0;
	word_index = 0;
	while (cmd[i])
	{
		while (is_whitespace(cmd[i]))
			i++;
		if (cmd[i])
		{
			str = get_str(cmd, &i);
			substr[word_index++] = str;
		}
	}
	substr[word_index] = NULL;
	return (substr);
}
