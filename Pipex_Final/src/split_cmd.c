/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:33 by clu               #+#    #+#             */
/*   Updated: 2025/03/10 09:58:53 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**check_cmd(char *cmd, int *cmd_count);
static void	free_cmds(char **cmds, int count);

// Extract the string from the command
	// Validate and allocate memory for the command array
	// Extract the string from the command
char	**split_cmd(char *cmd)
{
	char	**cmds;
	int		i;
	int		cmd_index;
	int		cmd_count;

	cmds = check_cmd(cmd, &cmd_count);
	i = 0;
	cmd_index = 0;
	while (cmd[i])
	{
		while (ft_is_whitespace(cmd[i]))
			i++;
		if (cmd[i] != '\0')
		{
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

// Check and allocate memory for the command
	// Count the number of commands in the command string
	// Allocate memory for the command array
static char	**check_cmd(char *cmd, int *cmd_count)
{
	char	**result;

	if (!cmd || cmd[0] == '\0')
		cmd_error(cmd);
	*cmd_count = count_cmds(cmd);
	result = malloc(sizeof(char *) * (*cmd_count + 1));
	if (!result)
		ft_pipex_error("pipex: memory allocation failed", 1);
	return (result);
}

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
