/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/02/28 15:01:36 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Prepare the command for execution
char	*prep_cmd_path(char *cmd, char **envp, t_pipex *pipex)
{
	int		i;
	char	*prep_cmd;

	i = 0;
	while (!ft_strnstr(envp[i], "PATH=", 5))
		i++;
	pipex->envp_paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (pipex->envp_paths[i])
	{
		prep_cmd = ft_strjoin(pipex->envp_paths[i], "/");
		pipex->cmd_path = ft_strjoin(prep_cmd, cmd);
		free (prep_cmd);
		if (access(pipex->cmd_path, F_OK) == 0)
		{
			ft_free_array(pipex->envp_paths);
			return (pipex->cmd_path);
		}
		free(pipex->cmd_path);
		i++;
	}
	ft_free_array(pipex->envp_paths);
	cmd_error(cmd);
	return (NULL);
}

// Execute the command
void	exec_cmd(char *cmd, char **envp, t_pipex *pipex)
{
	char	**cmds;

	cmds = ft_split(cmd, ' ');
	if (!cmds)
		exit_error();
	pipex->cmd_path = prep_cmd_path(cmds[0], envp, pipex);
	if (!pipex->cmd_path)
	{
		ft_free_array(cmds);
		free(pipex->cmd_path);
		exit(127);
	}
	if (execve(pipex->cmd_path, cmds, envp) == -1)
	{
		ft_free_array(cmds);
		free(pipex->cmd_path);
		exit_error();
	}
}
