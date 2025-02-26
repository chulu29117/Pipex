/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_for_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:49:49 by clu               #+#    #+#             */
/*   Updated: 2025/02/26 12:07:48 by clu              ###   ########.fr       */
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

// skip_quotes detects the " and sets active quote to "
static void	skip_quotes(char *cmd, int *i, char *quote)
{
	*quote = 0;
	if (cmd[*i] == '\'' || cmd[*i] == '\"')
	{
		*quote = cmd[*i];
		(*i)++;
	}
}

// Count the number of elements in the command string
int	count_cmds(char *cmd)
{
	int		i;
	int		cmd_count;
	char	quote;

	i = 0;
	cmd_count = 0;
	quote = 0;
	while (cmd[i])
	{
		// Skip the leading whitespaces
		while (cmd[i] && ft_is_whitespace(cmd[i]))
			i++;
		if (cmd[i])
		{
			// Count becomes 1 when a character is found
			cmd_count++;
			// Run until a whitespace is found or until a quote is found
			while (cmd[i] && (!ft_is_whitespace(cmd[i]) || quote))
			{
				// Check if the character is a quote character
				// Continues until the closing quote is found
				check_quote(&quote, cmd[i]);
				i++;
			}
		}
	}
	return (cmd_count);
}

// Copy the quoted string to the buffer
static void	prep_substr(char *cmd, int *i, char quote, t_buffer *buf)
{
	while (cmd[*i])
	{
		if (quote)
		{
			// If an escaped quote if found and the next character is a quote
			// Skip the escaped quote
			if (cmd[*i] == '\\' && cmd[*i + 1] == quote)
				(*i)++;
			// If the closing quote is found, break the loop
			else if (cmd[*i] == quote)
			{
				(*i)++;
				break ;
			}
		}
		else if (ft_is_whitespace(cmd[*i]))
			break ;
		// Copy the character to the buffer
		buf->result[buf->pos++] = cmd[*i];
		(*i)++;
		// If the buffer is full, reallocate memory
		if (buf->pos >= buf->capacity - 1)
		{
			buf->capacity *= 2;
			buf->result = ft_realloc(buf->result, buf->pos, buf->capacity);
			if (!buf->result)
				ft_pipex_error("pipex: realloc failed", 1);
		}
	}
}

char	*extract_str(char *cmd, int *i)
{
	t_buffer	buf;
	char		quote;

	buf.capacity = 64;
	buf.pos = 0;
	buf.result = malloc(buf.capacity);
	if (!buf.result)
		ft_pipex_error("pipex: malloc failed", 1);
	// Skip the leading whitespaces
	while (cmd[*i] && ft_is_whitespace(cmd[*i]))
		(*i)++;
	quote = 0;
	// skip_quotes detects the " and sets active quote to "
	skip_quotes(cmd, i, &quote);
	// Copy the quoted string to the buffer
	prep_substr(cmd, i, quote, &buf);
	buf.result[buf.pos] = '\0';
	return (buf.result);
}


// int	count_cmds(char *cmd)
// {
//	int		i;
//	int		count;
//	char	quote;

//	i = 0;
//	count = 0;
//	while (cmd[i])
//	{
//		while (cmd[i] && ft_is_whitespace(cmd[i]))
//			i++;
//		if (cmd[i])
//		{
//			count++;
//			if (cmd[i] == '\'' || cmd[i] == '\"')
//			{
//				quote = cmd[i++];
//				while (cmd[i] && cmd[i] != quote)
//					i++;
//				if (cmd[i])
//					i++;
//			}
//			else
//			{
//				while (cmd[i] && !ft_is_whitespace(cmd[i]))
//					i++;
//			}
//		}
//	}
//	return (count);
// }
