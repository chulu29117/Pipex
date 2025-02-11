/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:22:18 by clu               #+#    #+#             */
/*   Updated: 2025/02/11 18:49:36 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	init_bonus(t_bonus *bonus, int argc, char **argv, char **envp)
{
	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		bonus->infile_fd = here_doc(argv[2]);
		bonus->outfile_fd = open(argv[argc - 1],
				O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (bonus->outfile_fd < 0)
			ft_pipex_error("Error opening output file", 1);
		bonus->cmd_count = argc - 4;
		bonus->argv = &argv[3];
	}
	else
	{
		bonus->infile_fd = open(argv[1], O_RDONLY);
		bonus->outfile_fd = open(argv[argc - 1],
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (bonus->infile_fd < 0 || bonus->outfile_fd < 0)
			ft_pipex_error("Error opening input/output file", 1);
		bonus->cmd_count = argc - 3;
		bonus->argv = &argv[2];
	}
	bonus->envp = envp;
	bonus->pipes = alloc_pipes(bonus->cmd_count - 1);
	if (!bonus->pipes)
		ft_pipex_error("Error allocating pipes", 1);
}

static void	child_process_bonus(t_bonus *bonus, int i)
{
	char	**cmd_args;
	char	*cmd_path;
	int		j;

	if (i == 0)
	{
		dup2(bonus->infile_fd, STDIN_FILENO);
		dup2(bonus->pipes[0][1], STDOUT_FILENO);
	}
	else if (i == bonus->cmd_count - 1)
	{
		dup2(bonus->pipes[i - 1][0], STDIN_FILENO);
		dup2(bonus->outfile_fd, STDOUT_FILENO);
	}
	else
	{
		dup2(bonus->pipes[i - 1][0], STDIN_FILENO);
		dup2(bonus->pipes[i][1], STDOUT_FILENO);
	}
	j = 0;
	while (j < bonus->cmd_count - 1)
	{
		close(bonus->pipes[j][0]);
		close(bonus->pipes[j][1]);
		j++;
	}
	cmd_args = split_cmd(bonus->argv[i]);
	if (!cmd_args)
		ft_pipex_error("Error splitting command", 1);
	cmd_path = find_path(cmd_args[0], bonus->envp);
	if (!cmd_path)
	{
		cmd_error(cmd_args[0]);
		exit(127);
	}
	execve(cmd_path, cmd_args, bonus->envp);
	ft_pipex_error("Error executing command", 1);
}

static void	fork_and_exec_bonus(t_bonus *bonus)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < bonus->cmd_count)
	{
		pid = fork();
		if (pid < 0)
			ft_pipex_error("Error forking", 1);
		if (pid == 0)
			child_process_bonus(bonus, i);
		i++;
	}
}

static void	close_bonus_pipes(t_bonus *bonus)
{
	int	i;

	i = 0;
	while (i < bonus->cmd_count - 1)
	{
		close(bonus->pipes[i][0]);
		close(bonus->pipes[i][1]);
		i++;
	}
}

static void	wait_bonus_children(t_bonus *bonus)
{
	int	i;
	int	status;

	i = 0;
	while (i < bonus->cmd_count)
	{
		wait(&status);
		i++;
	}
}

void	exec_bonus(int argc, char **argv, char **envp)
{
	t_bonus	bonus;

	init_bonus(&bonus, argc, argv, envp);
	fork_and_exec_bonus(&bonus);
	close_bonus_pipes(&bonus);
	wait_bonus_children(&bonus);
}
