/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 11:30:09 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = split_cmd(cmd);
	if (!args || !args[0])
	{
		ft_putstr_fd("pipex: command not found\n", 2);
		exit(127);
	}
	path = find_path(args[0], envp);
	if (!path)
	{
		ft_putstr_fd("pipex: command not found\n", 2);
		exit(127);
	}
	if (execve(path, args, envp) == -1)
	{
		perror("Execution failed");
		if (errno == ENOENT)
			exit(127);
		if (errno == EACCES)
			exit(126);
		exit(1);
	}
}
