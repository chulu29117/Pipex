/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 00:02:04 by clu               #+#    #+#             */
/*   Updated: 2025/02/11 17:15:18 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if ((ft_strcmp(argv[1], "here_doc") == 0 && argc < 6)
		|| (ft_strcmp(argv[1], "here_doc") != 0 && argc < 5))
	{
		errno = EINVAL;
		pipex_error("Usage: ./pipex here_doc LIMITER cmd1 ... file OR\n"
			"./pipex file1 cmd1 cmd2 ... file2", 1);
	}
}
