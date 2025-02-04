/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 15:49:48 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int prepare_cmd(char *cmd, char ***args, char **path, char **envp)
{
	*args = split_cmd(cmd);
	if (!*args || !(*args)[0])
	{
		ft_putstr_fd("pipex: command not found\n", 2);
		ft_free_array(*args);
		return (127);
	}
	*path = find_path((*args)[0], envp);
	if (!*path)
	{
		ft_putstr_fd("pipex: command not found\n", 2);
		ft_free_array(*args);
		return (127);
	}
	return (0);
}

void exec_cmd(char *cmd, char **envp)
{
	char **args;
	char *path;
	int exit_code;

	exit_code = prepare_cmd(cmd, &args, &path, envp);
	if (exit_code != 0)
		exit(exit_code);
	if (execve(path, args, envp) == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		free(path);
		ft_free_array(args);
		if (errno == ENOENT)
			exit_code = 127;
		else if (errno == EACCES)
			exit_code = 126;
		else
			exit_code = 1;
		exit(exit_code);
	}
}
