/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 18:32:35 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/16 18:32:35 by yunguo           ###   ########.fr       */
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
	free(ast->data); // free arr
	//free redir
	free(ast);
}
