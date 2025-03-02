/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:33 by clu               #+#    #+#             */
/*   Updated: 2025/02/23 23:40:45 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Free the cmds array
static void	free_cmds(char **cmds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(cmds[i]);
		i++;
	}
	free(cmds);
}

// Check and allocate memory for the command
static char	**check_cmd(char *cmd, int *i, int *cmd_count)
{
	char	**result;

	if (!cmd || cmd[0] == '\0')
		ft_pipex_error("pipex: command not found\n", 127);
	// Count the number of commands in the command string
	*cmd_count = count_cmds(cmd);
	// Allocate memory for the command array
	result = malloc(sizeof(char *) * (*cmd_count + 1));
	if (!result)
		ft_pipex_error("pipex: memory allocation failed", 1);
	*i = 0;
	return (result);
}

// Extract the string from the command
char	**split_cmd(char *cmd)
{
	char	**cmds;
	int		i;
	int		cmd_index;
	int		cmd_count;

	// Validate and allocate memory for the command array
	cmds = check_cmd(cmd, &i, &cmd_count);
	i = 0;
	cmd_index = 0;
	while (cmd[i])
	{
		while (is_whitespace(cmd[i]))
			i++;
		if (cmd[i] != '\0')
		{
			// Extract the string from the command
			cmds[cmd_index] = extract_str(cmd, &i);
			if (!cmds[cmd_index])
			{
				free_cmds(cmds, cmd_index);
				ft_pipex_error("pipex: memory allocation failed", 1);
			}
			cmd_index++;
		}
	}
	cmds[cmd_index] = NULL;
	return (cmds);
}
