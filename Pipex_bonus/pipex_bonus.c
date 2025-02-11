/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 00:02:04 by clu               #+#    #+#             */
/*   Updated: 2025/02/11 18:30:41 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc < 5)
	{
		write(2, "Usage: ./pipex [infile/here_doc] "
				"[cmd1 ... cmdn] [outfile]\n", 54);
		return (1);
	}
	exec_bonus(argc, argv, envp);
	return (0);
}
