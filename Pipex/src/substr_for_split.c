/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_for_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:49:49 by clu               #+#    #+#             */
/*   Updated: 2025/02/26 11:22:05 by clu              ###   ########.fr       */
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
