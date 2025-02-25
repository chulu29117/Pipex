/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_for_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:49:49 by clu               #+#    #+#             */
/*   Updated: 2025/02/25 21:14:33 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Append a character to the string
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

static void	process_quoted_substr(char *cmd, int *i, char **res,
	int *capacity, int *pos)
{
	char	quote = cmd[*i];
	char	*seg;
	int		seg_len;

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

static void	process_substr(char *cmd, int *i, char **res,
	int *capacity, int *pos)
{
	if (cmd[*i] == '\'' || cmd[*i] == '\"')
	{
		process_quoted_substr(cmd, i, res, capacity, pos);
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
