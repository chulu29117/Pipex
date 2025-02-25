/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_for_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:49:49 by clu               #+#    #+#             */
/*   Updated: 2025/02/25 18:02:28 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Check if the character is a quote character
static void	update_quote(char c, char *quote, int *in_string, int *cmd_count)
{
	if (*quote == 0 && (c == '\'' || c == '\"'))
		*quote = c;
	else if (*quote == c)
		*quote = 0;
	if (!ft_is_whitespace(c) || quote != 0)
	{
		if (in_string == 0)
		{
			cmd_count++;
			*in_string = 1;
		}
	}
	else
		*in_string = 0;
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
		if (cmd[i] == '\\' && cmd[i + 1])
		{
			i += 2;
			continue ;
		}
		update_quote(cmd[i], &quote, &in_string, &cmd_count);
		i++;
	}
	return (cmd_count);
}

// static void	check_substr(char *cmd, int *i, char quote, int *len)
// {
// 	while (cmd[*i] && (quote != 0 || !ft_is_whitespace(cmd[*i])))
// 	{
// 		if (cmd[*i] == '\\' && cmd[*i + 1])
// 		{
// 			(*i)++;
// 			len++;
// 		}
// 		else if (quote != 0 && cmd[*i] == quote)
// 			break ;
// 		(*i)++;
// 		(*len)++;
// 	}
// }

// char	*extract_str(char *cmd, int *i)
// {
// 	int		start;
// 	int		len;
// 	char	quote;
// 	char	*substr;

// 	start = *i;
// 	len = 0;
// 	quote = 0;
// 	if (cmd[*i] == '\'' || cmd[*i] == '\"')
// 	{
// 		quote = cmd[*i];
// 		start++;
// 		(*i)++;
// 	}
// 	check_substr(cmd, i, quote, &len);
// 	if (quote != 0 && cmd[*i] == quote)
// 		(*i)++;
// 	substr = ft_substr(cmd, start, len);
// 	if (!substr)
// 		ft_pipex_error("pipex: ft_substr failed", 1);
// 	return (substr);
// }

static void	append_char(char **str, int *cap, int *pos, char c)
{
	char	*tmp;

	if (*pos + 1 >= *cap)
	{
		*cap *= 2;
		tmp = realloc(*str, *cap);
		if (!tmp)
			ft_pipex_error("pipex: memory allocation failed", 1);
		*str = tmp;
	}
	(*str)[(*pos)++] = c;
}

char	*extract_quoted(char *cmd, int *i, char quote)
{
	int		cap = 64, pos = 0;
	char	*res = malloc(cap);

	if (!res) ft_pipex_error("pipex: memory allocation failed", 1);
	while (cmd[*i])
	{
		if (cmd[*i] == '\\' && cmd[*i + 1] == quote)
		{
			(*i)++;
			append_char(&res, &cap, &pos, cmd[*i]);
		}
		else if (cmd[*i] == quote)
		{
			(*i)++;
			break;
		}
		else
			append_char(&res, &cap, &pos, cmd[*i]);
		(*i)++;
	}
	res[pos] = '\0';
	return (res);
}

static void	process_substr(char *cmd, int *i, char **res,
	int *capacity, int *pos)
{
char	quote;
char	*seg;
int		seg_len;

if (cmd[*i] == '\'' || cmd[*i] == '\"')
{
	quote = cmd[*i];
	(*i)++; /* skip opening quote */
	seg = extract_quoted(cmd, i, quote);
	seg_len = ft_strlen(seg);
	while (*pos + seg_len >= *capacity)
	{
		*capacity *= 2;
		*res = realloc(*res, *capacity);
		if (!*res)
			ft_pipex_error("pipex: memory allocation failed", 1);
	}
	ft_strlcpy(*res + *pos, seg, seg_len + 1);
	*pos += seg_len;
	free(seg);
}
else if (cmd[*i] == '\\' && cmd[*i + 1])
{
	(*i)++; /* skip backslash */
	append_char(res, capacity, pos, cmd[*i]);
	(*i)++;
}
else
{
	append_char(res, capacity, pos, cmd[*i]);
	(*i)++;
}
}

char	*extract_str(char *cmd, int *i)
{
	int		capacity = 64;
	int		pos = 0;
	char	*result;

	result = malloc(capacity);
	if (!result)
		ft_pipex_error("pipex: memory allocation failed", 1);
	while (cmd[*i] && !ft_is_whitespace(cmd[*i]))
	{
		process_substr(cmd, i, &result, &capacity, &pos);
	}
	result[pos] = '\0';
	return (result);
}
