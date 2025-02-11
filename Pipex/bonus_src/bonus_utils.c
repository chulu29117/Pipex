/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:30:05 by clu               #+#    #+#             */
/*   Updated: 2025/02/11 18:30:08 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** alloc_pipes() allocates an array of pipes.
** Each pipe is an int[2] created with pipe().
*/
int	**alloc_pipes(int count)
{
	int	**pipes;
	int	i;

	pipes = (int **)malloc(sizeof(int *) * count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < count)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		if (pipe(pipes[i]) < 0)
			ft_pipex_error("Error creating pipe", 1);
		i++;
	}
	return (pipes);
}

/*
** child_setup() duplicates file descriptors according to the command index.
*/
void	child_setup(t_bonus *bonus, int i)
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
