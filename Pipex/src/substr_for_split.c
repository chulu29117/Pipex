/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_for_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:49:49 by clu               #+#    #+#             */
/*   Updated: 2025/02/25 23:15:14 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Check if the character is a quote character
static void	check_quote(char *quote, char c)
{
	if (*quote == 0 && (c == '\'' || c == '\"'))
		*quote = c;
	else if (*quote == c)
		*quote = 0;
}

// Count the number of commands in the command string
int	count_cmds(char *cmd)
{
	int		i;
	int		cmd_count;
	int		in_string;
	char	quote;

	i = 0;
	cmd_count = 0;
	in_string = 0;
	quote = 0;
	while (cmd[i])
	{
		check_quote(&quote, cmd[i]);
		if (!ft_is_whitespace(cmd[i]) || quote)
		{
			if (!in_string)
			{
				cmd_count++;
				in_string = 1;
			}
		}
		else
			in_string = 0;
		i++;
	}
	return (cmd_count);
}

// Skip quotes in the command string
static void	skip_quotes(char *cmd, int *i, char *quote)
{
	*quote = 0;
	if (cmd[*i] == '\'' || cmd[*i] == '\"')
	{
		*quote = cmd[*i];
		(*i)++;
	}
}

char	*extract_str(char *cmd, int *i)
{
	int		capacity = 64;
	int		pos = 0;
	char	*result;
	char	quote = 0;

	result = malloc(capacity);
	if (!result)
		ft_pipex_error("pipex: malloc failed", 1);
	while (cmd[*i] && ft_is_whitespace(cmd[*i]))
		(*i)++;
	skip_quotes(cmd, i, &quote);
	while (cmd[*i])
	{
		if (quote)
		{
			if (cmd[*i] == '\\' && cmd[*i + 1] == quote)
				(*i)++;
			else if (cmd[*i] == quote)
			{
				(*i)++;
				break ;
			}
		}
		else if (ft_is_whitespace(cmd[*i]))
		{
			break ;
		}
		result[pos++] = cmd[*i];
		(*i)++;
		if (pos >= capacity - 1)
		{
			capacity *= 2;
			result = ft_realloc(result, pos, capacity);
			if (!result)
				ft_pipex_error("pipex: realloc failed", 1);
		}
	}
	result[pos] = '\0';
	return (result);
}
