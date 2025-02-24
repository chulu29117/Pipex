/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_for_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:49:49 by clu               #+#    #+#             */
/*   Updated: 2025/02/24 10:58:13 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Check if the character is a whitespace character
int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

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
		if (!is_whitespace(cmd[i]) || quote != 0)
		{
			if (in_string == 0)
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
static void	skip_quotes(char *cmd, int *i, char *quote, int *start)
{
	*quote = 0;
	// If the character is a quote character
	if (cmd[*i] == '\'' || cmd[*i] == '\"')
	{
		// Set the quote character
		*quote = cmd[*i];
		(*start)++;
		(*i)++;
	}
}

// Extract the string from the command
char	*extract_str(char *cmd, int *i)
{
	int		start;
	int		len;
	char	quote;
	char	*substr;

	start = *i;
	len = 0;
	// Skip quotes in the command string
	skip_quotes(cmd, i, &quote, &start);
	// While the character is not a whitespace character or a quote character
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
		ft_pipex_error("pipex: ft_substr failed", 1);
	return (substr);
}
