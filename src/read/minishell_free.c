/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 09:57:27 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/13 10:48:14 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_ast(t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->left != NULL)
		free_ast(ast->left);
	if (ast->riht != NULL)
		free_ast(ast->riht);
	free(ast->data);
	free(ast);
}

void	free_token_lst(t_token **lst)
{
	t_token	*tmp;
	t_token	*cur;

	cur = *lst;
	while (cur)
	{
		ft_sfree(&cur->text);
		tmp = cur;
		cur = cur->next;
		ft_sfree(&tmp);
	}
	*lst = NULL;
}

void	free_null_var(t_ast **ast, char ***env, char **line, t_token **lexed)
{
	if (ast != NULL)
	{
		free_ast(*ast);
		*ast = NULL;
	}
	if (env != NULL)
	{
		free_arr(*env);
		*env = NULL;
	}
	if (line != NULL)
	{
		ft_sfree(line);
		*line = NULL;
	}
	if (lexed != NULL)
	{
		free_token_lst(*lexed);
		*lexed = NULL;
	}
}

void	free_ms_var(t_ast **astree, t_info *i, char *cmd)
{
	if (ft_strcmp(cmd, "tmp") == 0)
	{
		free_null_var(astree, NULL, &i->line, &i->lexed);
	}
	else if (ft_strcmp(cmd, "all") == 0)
	{
		free_null_var(astree, &i->my_env, &i->line, &i->lexed);
	}
}
