/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 18:32:35 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/26 19:31:53 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_rdir(t_redir **rdir)
{
	t_redir	*curr;
	t_redir	*temp;

	curr = *rdir;
	while (curr)
	{
		ft_sfree((void **)&curr->file);
		temp = curr;
		curr = curr->next;
		ft_sfree((void **)&temp);
	}
	curr = NULL;
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
	t_ast	*cur;

	cur = *one;
	cur->left = NULL;
	cur->riht = NULL;
	cur->type = 0;
	free_arr(cur->args);
	cur->args = NULL;
	free_rdir(&cur->rdir);
	cur->rdir = NULL;
	ft_sfree((void **)one);
}
