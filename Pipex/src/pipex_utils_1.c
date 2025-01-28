/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:33 by clu               #+#    #+#             */
/*   Updated: 2025/01/28 14:49:00 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = split_command(cmd);
	if (!args || !args[0])
		ft_error("Error: Invalid command");
	path = find_path(args[0], envp);
	if (!path)
		ft_error("Error: Command not found");
	if (execve(path, args, envp) == -1)
		ft_error("Error: Command execution failed");
}

void	first_child(t_pipex *pipex, char *cmd, char **envp)
{
	dup2(pipex->infile, STDIN_FILENO);
	dup2(pipex->pipe_fds[1], STDOUT_FILENO);
	close(pipex->pipe_fds[0]);
	exec_cmd(cmd, envp);
}

void	second_child(t_pipex *pipex, char *cmd, char **envp)
{
	dup2(pipex->pipe_fds[0], STDIN_FILENO);
	dup2(pipex->outfile, STDOUT_FILENO);
	close(pipex->pipe_fds[1]);
	exec_cmd(cmd, envp);
}

void	close_pipes_and_wait(t_pipex *pipex)
{
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	close(pipex->infile);
	close(pipex->outfile);
	waitpid(pipex->pid1, NULL, 0);
	waitpid(pipex->pid2, NULL, 0);
}
