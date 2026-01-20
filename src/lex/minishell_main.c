/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:29:03 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/17 11:33:31 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

	// 		parse_line(line, &parsed)
	//		take care of i.parsed == NULL
	// 		astree = build_ast(parsed);
	// free NULL
t_bool	line_valid(char **line)
{
	char	*temp;

	if (!line || !*line)
		return (FALSE);
	temp = ft_strtrim(*line, " \t\n\r\v\f");
	if (!temp)
		return (FALSE);
	ft_sfree(line);
	*line = temp;
	if (ft_strcmp(*line, "") == 0)
		return (FALSE);
	if (line_is_complete(*line) == FALSE)
		return (FALSE);
	return (TRUE);
}
	
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
			if (parse_line(i.line, &i.parsed) == 0)
				astree = build_ast(i.parsed);
			if (astree)
				run_cmd(astree, i);
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
// 	char	*parsed;

// 	nullise_var(&astree, &my_env, &line, &parsed);
// 	if (argc != 1 || ft_arrdup_mk2(&my_env, envp) != 0)
// 		return (free_null_var(NULL, &my_env, NULL, NULL), 2);
// 	while (TRUE)
// 	{
// 		line = read_multiline("moonshell> ");
// 		//if line has error then freenullvar then exit.
// 		if (!line || ft_strcmp(line, "exit") == 0)
// 			return (free_null_var(&astree, &my_env, &line, &parsed), 0);
// 		if (line != NULL)
// 			add_history(line);
// 		if (line && parse_line(line, &parsed) == 0)
// 		{
// 			astree = build_ast(parsed);
// 			if (astree)
// 				run_cmd(astree);
// 		}
// 		free_null_var(&astree, NULL, &line, &parsed);
// 	}
// 	return (free_null_var(&astree, &my_env, NULL, NULL), 0);
// }
