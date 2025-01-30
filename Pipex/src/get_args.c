/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:33 by clu               #+#    #+#             */
/*   Updated: 2025/01/30 22:24:54 by clu              ###   ########.fr       */
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
		if (!is_whitespace(cmd[i]) && (i == 0 || is_whitespace(cmd[i - 1])) && !quotes)
			count++;
		i++;
	}
	return (count);
}

static char	*extract_str(char *cmd, int *i)
{
	int		start;
	int		len;
	char	quote;
	char	*str;

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
		(*i)++;
	}
	else
	{
		while (cmd[*i] && !is_whitespace(cmd[*i]))
			(*i)++;
		len = *i - start;
	}
	str = ft_substr(cmd, start, len);
	return (str);
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
			str = extract_token(cmd, &i);
			substr[word_index++] = str;
		}
	}
	substr[word_index] = NULL;
	return (substr);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
		envp++;
	if (!*envp)
		return (NULL);
	paths = ft_split(*envp + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, cmd);
		free(path);
		if (access(full_path, X_OK) == 0)
			return (ft_free_array(paths), full_path);
		free(full_path);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}
