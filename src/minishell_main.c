/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:29:03 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/13 08:30:31 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_ast	*astree;
	char	**my_env;
	char	*line;
	char	*parsed;

	nullise_var(&astree, &my_env, &line, &parsed);
	if (argc != 1 || init_env(&my_env, envp) != 0)
		return (clearall(NULL, &my_env, NULL, NULL), 2);
	while (TRUE)
	{
		line = read_multiline("moonshell> ");
		if (!line || ft_strcmp(line, "exit") == 0)
			return (clearall(&astree, &my_env, &line, &parsed), 0);
		if (line != NULL)
			add_history(line);
		if (line && parse_line(line, &parsed) == 0)
		{
			astree = build_ast(parsed);
			if (astree)
				run_cmd(astree);
		}
		clearall(&astree, NULL, &line, &parsed);
	}
	return (clearall(&astree, &my_env, NULL, NULL), 0);
}
