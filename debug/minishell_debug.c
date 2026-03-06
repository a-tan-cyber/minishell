/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_debug.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:14:20 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/06 19:25:59 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"
#include <stdio.h>

static void	ft_print_arr(char *name, const char **arr)
{
	size_t	i;

	printf("%s: {", name);
	if (!arr)
	{
		printf("}\n");
		return ;
	}
	i = 0;
	while (arr[i])
	{
		printf("%s, ", arr[i]);
		i++;
	}
	printf("}\n");
}

static const char	*ms_oper_str(t_oper op)
{
	if (op == REDI_IN)
		return ("<");
	if (op == REDI_OT)
		return (">");
	if (op == APPEND)
		return (">>");
	if (op == HEREDOC)
		return ("<<");
	if (op == PIPE)
		return ("|");
	if (op == AND)
		return ("&&");
	if (op == OR)
		return ("||");
	if (op == PAREN_L)
		return ("(");
	if (op == PAREN_R)
		return (")");
	if (op == SPCE)
		return ("SPCE");
	if (op == TEXT)
		return ("TEXT");
	return ("?");
}

static void	ft_print_rdir(char *name, t_redir *rdir)
{
	int	first;

	printf("%s: {", name);
	first = 1;
	while (rdir)
	{
		if (!first)
			printf(", ");
		printf("%s", ms_oper_str((t_oper)rdir->type));
		if (rdir->file)
			printf(" %s", rdir->file);
		first = 0;
		rdir = rdir->next;
	}
	printf("}\n");
}

void	print_astree(int level, t_ast *ast)
{
	char	name[32];

	if (!ast)
		return ;
	printf("type lvl %d: %d\n", level, (int)ast->type);
	ft_strcpy(name, "args lvl ");
	ft_itoa_fast(level, name + 9);
	ft_print_arr(name, (const char **)ast->args);
	if (ast->type == AST_CMD)
	{
		ft_strcpy(name, "rdir lvl ");
		ft_itoa_fast(level, name + 9);
		ft_print_rdir(name, ast->rdir);
	}
	if (ast->left)
		print_astree(level + 1, ast->left);
	if (ast->riht)
		print_astree(level + 1, ast->riht);
}
