/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:15:11 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/18 18:15:11 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_ast	*astree;
	t_info	i;

	init_ms_var(&astree, &i);
	signal(SIGINT, int_handler);
	signal(SIGQUIT, SIG_IGN);
	if (argc != 1 || ft_arrdup_mk2(&i.my_env, envp) != 0)
		return (free_ms_var(&astree, &i, "all"), 2);
	while (TRUE)
	{
		i.line = read_multiline("moonshell> ");
		if (!i.line || !ft_strcmp(i.line, "exit\n"))
			return (free_ms_var(&astree, &i, "all"), 0);
		if (line_valid(&i.line))
		{
			add_history(i.line);
			if (lex_line(i.line, &i.lexed, i.my_env, &i) == 0)
				astree = build_ast_rec(i.lexed, token_last(i.lexed));
			if (astree)
				print_astree(0, astree);
				// run_cmd(astree, i);
		}
		free_ms_var(&astree, &i, "tmp");
	}
	return (free_ms_var(&astree, &i, "all"), 0);
}
// int	main(int argc, char **argv, char **envp)
// {
// 	t_ast	*astree;
// 	char	**my_env;
// 	char	*line;
// 	char	*lexed;

// 	nullise_var(&astree, &my_env, &line, &lexed);
// 	if (argc != 1 || ft_arrdup_mk2(&my_env, envp) != 0)
// 		return (free_null_var(NULL, &my_env, NULL, NULL), 2);
// 	while (TRUE)
// 	{
// 		line = read_multiline("moonshell> ");
// 		//if line has error then freenullvar then exit.
// 		if (!line || ft_strcmp(line, "exit") == 0)
// 			return (free_null_var(&astree, &my_env, &line, &lexed), 0);
// 		if (line != NULL)
// 			add_history(line);
// 		if (line && lex_line(line, &lexed) == 0)
// 		{
// 			astree = build_ast(lexed);
// 			if (astree)
// 				run_cmd(astree);
// 		}
// 		free_null_var(&astree, NULL, &line, &lexed);
// 	}
// 	return (free_null_var(&astree, &my_env, NULL, NULL), 0);
// }
