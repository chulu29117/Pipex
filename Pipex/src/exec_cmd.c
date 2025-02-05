/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/02/05 11:47:56 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	prepare_cmd(char *cmd, char ***args, char **path, char **envp)
{
	*args = split_cmd(cmd);
	if (!*args)
		pipex_error("pipex: memory allocation failed");
	if (!(*args)[0])
	{
		ft_free_array(*args);
		cmd_error(cmd);
		return (127);
	}
	*path = find_path((*args)[0], envp);
	if (!*path)
	{
		ft_free_array(*args);
		cmd_error(cmd);
		return (127);
	}
	return (0);
}

void	exec_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;
	int		exit_code;

	exit_code = prepare_cmd(cmd, &args, &path, envp);
	if (exit_code != 0)
		exit(exit_code);
	if (execve(path, args, envp) == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		free(path);
		ft_free_array(args);
		if (errno == ENOENT)
			exit(127);
		else if (errno == EACCES)
			exit(126);
		else
			exit(1);
	}
}
