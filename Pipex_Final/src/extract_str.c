/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:49:49 by clu               #+#    #+#             */
/*   Updated: 2025/03/04 16:30:30 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	check_quote(char *quote, char c);
static void	skip_quotes(char *cmd, int *i, char *quote);
static void	prep_substr(char *cmd, int *i, char quote, t_buffer *buf);

// Extract the string from the command
	// Skip the leading whitespaces
	// Detect the quote character
	// Copy the quoted string to the buffer
char	*extract_str(char *cmd, int *i)
{
	t_buffer	buf;
	char		quote;

	buf.capacity = 64;
	buf.pos = 0;
	buf.result = malloc(buf.capacity);
	if (!buf.result)
		ft_pipex_error("pipex: malloc failed", 1);
	while (cmd[*i] && ft_is_whitespace(cmd[*i]))
		(*i)++;
	quote = 0;
	skip_quotes(cmd, i, &quote);
	prep_substr(cmd, i, quote, &buf);
	buf.result[buf.pos] = '\0';
	return (buf.result);
}

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

// Copy the quoted string to the buffer
	// If the character is a quote, skip the escaped quote
	// If the closing quote is found, break the loop
	// Copy the character to the buffer
	// If the buffer is full, reallocate memory
static void	prep_substr(char *cmd, int *i, char quote, t_buffer *buf)
{
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
			break ;
		buf->result[buf->pos++] = cmd[*i];
		(*i)++;
		if (buf->pos >= buf->capacity - 1)
		{
			buf->capacity *= 2;
			buf->result = ft_realloc(buf->result, buf->pos, buf->capacity);
			if (!buf->result)
				ft_pipex_error("pipex: realloc failed", 1);
		}
	}
}

// Count the number of elements in the command string
	// Skip the leading whitespaces
	// Count becomes 1 when a character is found
	// Run until a whitespace is found or until a quote is found
	// Check if the character is a quote character
	// Continues until the closing quote is found
	// Count the number of commands in the command string
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
		while (cmd[i] && ft_is_whitespace(cmd[i]))
			i++;
		if (cmd[i])
		{
			cmd_count++;
			while (cmd[i] && (!ft_is_whitespace(cmd[i]) || quote))
			{
				check_quote(&quote, cmd[i]);
				i++;
			}
		}
	}
	return (cmd_count);
}
