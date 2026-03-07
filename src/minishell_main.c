/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:15:11 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/07 18:46:10 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_setup(t_ast **ast, t_info *i, int argc, char **envp)
{
	init_ms_var(ast, i);
	if (argc != 1 || ms_var_bootstrap(i, envp) != 0)
		return (2);
	i->interactive = isatty(STDIN_FILENO);
	g_sig = 0;
	if (i->interactive)
	{
		rl_catch_signals = 0;
		rl_event_hook = rl_check_sigint;
		if (set_signals() != 0)
			return (2);
	}
	return (0);
}

static int	ms_handle_sigint(t_ast **ast, t_info *i)
{
	if (g_sig != SIGINT)
		return (0);
	g_sig = 0;
	i->err = 130;
	ft_sfree((void **)&i->line);
	free_ms_var(ast, i, "tmp");
	if (!i->interactive)
		return (0);
	return (1);
}

static int	ms_should_exit(t_info *i)
{
	if (!i->line)
	{
		if (i->interactive)
			write(1, "exit\n", 5);
		return (1);
	}
	return (0);
}

// debug ast by inserting print_astree(0, *ast); as the last line of this fn
// don't forget to uncomment the prototype in minishell.h
static void	ms_process_line(t_ast **ast, t_info *i)
{
	if (!line_valid(i, &i->line))
		return ;
	if (i->interactive && i->line && i->line[0] != '\0' && i->line[0] != '\n')
		ms_history_add(i, i->line);
	if (lex_line(i->line, &i->lexed, i) != 0)
		return ;
	*ast = build_ast_rec(i->lexed, token_last(i->lexed));
	if (!*ast)
	{
		ft_putendl_fd("moonshell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
		i->err = 2;
		return ;
	}
	ms_exec_ast(i, *ast);
}

int	main(int argc, char **argv, char **envp)
{
	t_ast	*astree;
	t_info	i;
	int		ret;

	(void)argv;
	ret = ms_setup(&astree, &i, argc, envp);
	if (ret != 0)
		return (free_ms_var(&astree, &i, "all"), ret);
	while (TRUE)
	{
		i.line = read_multiline(&i, "moonshell> ");
		if (ms_handle_sigint(&astree, &i))
			continue ;
		if (ms_should_exit(&i))
			return (free_ms_var(&astree, &i, "all"), i.err);
		ms_process_line(&astree, &i);
		if (i.exit_req)
			return (free_ms_var(&astree, &i, "all"), i.err);
		free_ms_var(&astree, &i, "tmp");
	}
	return (free_ms_var(&astree, &i, "all"), 0);
}
