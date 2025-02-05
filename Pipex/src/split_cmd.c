/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:33 by clu               #+#    #+#             */
/*   Updated: 2025/02/05 11:53:54 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_result(char **result, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(result[i]);
		i++;
	}
	free(result);
}

static char	**check_cmd(char *cmd, int *i, int *word_count)
{
	char	**result;

	if (!cmd || cmd[0] == '\0')
		pipex_error("pipex: command not found\n", 127);
	*word_count = count_words(cmd);
	result = malloc(sizeof(char *) * (*word_count + 1));
	if (!result)
		pipex_error("pipex: memory allocation failed", 1);
	*i = 0;
	return (result);
}

char	**split_cmd(char *cmd)
{
	char	**result;
	int		i;
	int		word_index;
	int		word_count;

	result = check_cmd(cmd, &i, &word_count);
	i = 0;
	word_index = 0;
	while (cmd[i])
	{
		while (is_whitespace(cmd[i]))
			i++;
		if (cmd[i] != '\0')
		{
			result[word_index] = extract_str(cmd, &i);
			if (!result[word_index])
			{
				free_result(result, word_index);
				pipex_error("pipex: memory allocation failed", 1);
			}
			word_index++;
		}
	}
	result[word_index] = NULL;
	return (result);
}
