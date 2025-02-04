/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:33 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 10:34:48 by clu              ###   ########.fr       */
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
	char	quote;
	int		in_word;

	i = 0;
	count = 0;
	quote = 0;
	in_word = 0;
	while (cmd[i])
	{
		if ((cmd[i] == '\'' || cmd[i] == '\"') && (!quote || quote == cmd[i]))
		{
			if (quote == 0)
				quote = cmd[i];
			else
				quote = 0;
		}
		if (!is_whitespace(cmd[i]) && quote != 0)
		{
			if (in_word == 0)
				count++;
			in_word = 1;
		}
		else
			in_word = 0;
		i++;
	}
	return (count);
}

static char	*extract_str(char *cmd, int *i, char quote)
{
	int		start;
	int		len;

	start = *i;
	len = 0;
	if (quote)
	{
		start++;
		(*i)++;
	}
	while (cmd[*i])
	{
		if (quote)
		{
			if (cmd[*i] == quote)
				break;
		}
		else if (is_whitespace(cmd[*i]))
			break;
		(*i)++;
		len++;
	}
	if (quote && cmd[*i])
		(*i)++;
	return (ft_substr(cmd, start, len));
}

static char	*get_str(char *cmd, int *i)
{
	char	quote;

	while (is_whitespace(cmd[*i]))
		(*i)++;
	quote = 0;
	if (cmd[*i] == '\'' || cmd[*i] == '\"')
		quote = cmd[*i];
	return (extract_str(cmd, i, quote));
}

char	**split_cmd(char *cmd)
{
	char	**substr;
	int		i;
	int		word_count;
	int		word_index;

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
		if (cmd[i] != '\0')
		{
			substr[word_index] = get_str(cmd, &i);
			word_index++;
		}
	}
	substr[word_index] = NULL;
	return (substr);
}
