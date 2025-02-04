/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:33 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 10:57:47 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

static int	count_words(char *cmd)
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
		if ((cmd[i] == '\'' || cmd[i] == '\"') && quote == 0)
			quote = cmd[i];
		else if (cmd[i] == quote)
			quote = 0;
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

static char	*extract_str(char *cmd, int *i)
{
	int		start;
	int		len;
	char	quote;

	start = *i;
	len = 0;
	quote = 0;
	if (cmd[*i] == '\'' || cmd[*i] == '\"')
	{
		quote = cmd[*i];
		(*i)++;
	}
	while (cmd[*i] && (quote != 0 || !is_whitespace(cmd[*i])))
	{
		if (quote != 0 && cmd[*i] == quote)
			break ;
		(*i)++;
		len++;
	}
	if (quote != 0 && cmd[*i] == quote)
		(*i)++;
	return (ft_substr(cmd, start, len));
}

char	**split_cmd(char *cmd)
{
	char	**result;
	int		i;
	int		word_index;
	int		word_count;

	if (!cmd || cmd[0] == '\0')
	{
		ft_putstr_fd("pipex: command not found\n", 2);
		exit(127);
	}
	word_count = count_words(cmd);
	result = malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	word_index = 0;
	while (cmd[i])
	{
		while (is_whitespace(cmd[i]))
			i++;
		if (cmd[i] != '\0')
			result[word_index++] = extract_str(cmd, &i);
	}
	result[word_index] = NULL;
	return (result);
}
