/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/01/30 22:25:00 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**get_cmd(char *cmd, char **envp, char **path)
{
	char	**args;

	args = split_cmd(cmd);
	if (!args || !args[0])
	{
		ft_free_array(args);
		sys_error("Error: Invalid command");
		exit(127);
	}
	*path = find_path(args[0], envp);
	if (!*path)
	{
		ft_free_array(args);
		sys_error("Error: Command not found");
		exit(127);
	}
	return (args);
}

void	exec_cmd(char *cmd, char **envp)
{
	char	*path;
	char	**args;

	args = get_cmd(cmd, envp, &path);
	if (execve(path, args, envp) == -1)
	{
		free(path);
		ft_free_array(args);
		sys_error("Error: Command execution failed");
		exit(errno);
	}
	free(path);
	ft_free_array(args);
}

void	first_child(t_pipex *pipex, char *cmd, char **envp)
{
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
	{
		close(pipex->infile);
		close(pipex->pipe_fds[0]);
		close(pipex->pipe_fds[1]);
		sys_error("Error: Unable to duplicate input file descriptor");
		exit(1);
	}
	if (dup2(pipex->pipe_fds[1], STDOUT_FILENO) == -1)
	{
		close(pipex->infile);
		close(pipex->pipe_fds[0]);
		close(pipex->pipe_fds[1]);
		sys_error("Error: Unable to duplicate pipe file descriptor");
		exit(1);
	}
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	close(pipex->infile);
	exec_cmd(cmd, envp);
}

void	second_child(t_pipex *pipex, char *cmd, char **envp)
{
	if (pipex->outfile == -1)
	{
		sys_error("Error: Unable to open output file");
		exit(1);
	}
	if (dup2(pipex->pipe_fds[0], STDIN_FILENO) == -1)
	{
		close(pipex->pipe_fds[0]);
		close(pipex->pipe_fds[1]);
		sys_error("Error: Unable to duplicate pipe file descriptor");
		exit(1);
	}
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
	{
		close(pipex->pipe_fds[0]);
		close(pipex->pipe_fds[1]);
		sys_error("Error: Unable to duplicate output file descriptor");
		exit(1);
	}
	close(pipex->pipe_fds[1]);
	close(pipex->pipe_fds[0]);
	if (pipex->outfile != -1)
		close(pipex->outfile);
	exec_cmd(cmd, envp);
	exit(1);
}

void	exec_pipe(t_pipex *pipex)
{
	int	status1;
	int	status2;

	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	if (pipex->infile != -1)
		close(pipex->infile);
	if (pipex->outfile != -1)
		close(pipex->outfile);
	if (waitpid(pipex->pid1, &status1, 0) == -1)
	{
		sys_error("Error: Waitpid failed");
		exit(1);
	}
	if (waitpid(pipex->pid2, &status2, 0) == -1)
	{
		sys_error("Error: Waitpid failed");
		exit(1);
	}
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	if (WIFEXITED(status1))
		exit(WEXITSTATUS(status1));
	exit(1);
}
