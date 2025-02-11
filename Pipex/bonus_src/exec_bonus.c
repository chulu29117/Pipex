/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:22:18 by clu               #+#    #+#             */
/*   Updated: 2025/02/11 17:27:04 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	child_setup(t_bonus *bonus, int i)
{
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
}

static int	**alloc_pipes(int count)
{
	int	**pipes;
	int	i;

	pipes = (int **)malloc(sizeof(int *) * count);
	if (pipes == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (pipes[i] == NULL)
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	exec_bonus(int argc, char **argv, char **envp)
{
	int	cmd_count;
	int	infile_fd;
	int	outfile_fd;
	int	**pipes;

	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		infile_fd = here_doc(argv[2]);
		outfile_fd = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (outfile_fd < 0)
			pipex_error("Error opening output file", 1);
		cmd_count = argc - 4;
	}
	else
	{
		infile_fd = open(argv[1], O_RDONLY);
		outfile_fd = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (infile_fd < 0 || outfile_fd < 0)
			pipex_error("Error opening input/output file", 1);
		cmd_count = argc - 3;
	}
	pipes = alloc_pipes(cmd_count - 1);
	if (pipes == NULL)
		pipex_error("Error allocating pipes", 1);
	
}
