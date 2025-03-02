/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/03/02 23:38:20 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// // Prepare the command for execution
// char	*prep_cmd_path(char *cmd, char **envp, t_pipex *pipex)
// {
// 	int		i;
// 	char	*prep_cmd;

// 	i = 0;
// 	while (!ft_strnstr(envp[i], "PATH=", 5))
// 		i++;
// 	pipex->envp_paths = ft_split(envp[i] + 5, ':');
// 	i = 0;
// 	while (pipex->envp_paths[i])
// 	{
// 		prep_cmd = ft_strjoin(pipex->envp_paths[i], "/");
// 		pipex->cmd_path = ft_strjoin(prep_cmd, cmd);
// 		free (prep_cmd);
// 		if (access(pipex->cmd_path, F_OK) == 0)
// 		{
// 			ft_free_array(pipex->envp_paths);
// 			return (pipex->cmd_path);
// 		}
// 		free(pipex->cmd_path);
// 		i++;
// 	}
// 	ft_free_array(pipex->envp_paths);
// 	cmd_error(cmd);
// 	return (NULL);
// }

// // Execute the command
// void	exec_cmd(char *cmd, char **envp, t_pipex *pipex)
// {
// 	char	**cmds;

// 	cmds = ft_split(cmd, ' ');
// 	if (!cmds)
// 		ft_pipex_error("pipex: memory allocation failed", 1);
// 	pipex->cmd_path = prep_cmd_path(cmds[0], envp, pipex);
// 	if (!pipex->cmd_path)
// 	{
// 		ft_free_array(cmds);
// 		free(pipex->cmd_path);
// 		exit(127);
// 	}
// 	if (execve(pipex->cmd_path, cmds, envp) == -1)
// 	{
// 		ft_free_array(cmds);
// 		free(pipex->cmd_path);
// 		cmd_error(cmd);
// 		exit(127);
// 	}
// }

// Prepare the command for execution
	// Split the command into an array of arguments
	// If the first argument is NULL, free the array and exit
	// Find the full path of the command
static int	prepare_cmd(char *cmd, char ***args, char **path, char **envp)
{
	*args = split_cmd(cmd);
	if (!*args)
		ft_pipex_error("pipex: memory allocation failed", 1);
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
		if (ft_strchr(cmd, '/') != NULL || cmd[0] == '.')
		{
			ft_putstr_fd("pipex: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
		else
			cmd_error(cmd);
		return(127);
	}
	return (0);
}

// Execute the command
	// Prepare the command (split arguments and find full path)
	// Execute the command
		// If the command is not found, exit with status 127
		// If the permission is denied, exit with status 126
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
		exit(127);
	}
}
