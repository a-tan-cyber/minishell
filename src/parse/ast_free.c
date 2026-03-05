/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 18:32:35 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/05 11:40:44 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_rdir(t_redir **rdir)
{
	t_redir	*curr;
	t_redir	*next;

	if (!rdir || !*rdir)
		return ;
	curr = *rdir;
	while (curr)
	{
		next = curr->next;
		ft_sfree((void **)&curr->file);
		ft_sfree((void **)&curr);
		curr = next;
	}
	*rdir = NULL;
}

void	free_ast(t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->left != NULL)
		free_ast(ast->left);
	if (ast->riht != NULL)
		free_ast(ast->riht);
	free_arr(ast->args);
	free_rdir(&ast->rdir);
	free(ast);
}

void	free_ast_one(t_ast **one)
{
	if (!one || !*one)
		return ;
	free_ast(*one);
	*one = NULL;
}
