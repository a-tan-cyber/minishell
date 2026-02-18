/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_debug.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:14:20 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/18 18:14:20 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_print_arr(char *name, const char **arr)
{
	size_t	i;

	printf("%s: {", name);
	i = 0;
	while (arr[i])
	{
		printf("%s, ", arr[i]);
		i++;
	}
	printf("}\n");
}

void	ft_print_rdir(char *name, t_redir *rdir)
{
	printf("%s: {", name);
	while (rdir)
	{
		printf(rdir->type);
		rdir = rdir->next;
	}
}

void	print_astree(int level, t_ast *ast)
{
	char	name[32];

	if (ast == NULL)
		return ;
	if (ast->left != NULL)
		print_astree(level + 1, ast->left);
	if (ast->riht != NULL)
		print_astree(level + 1, ast->riht);
	ft_strcpy(name, "level ");
	ft_itoa_fast(level, name + 6);
	ft_print_arr(name, ast->args);
	ft_print_rdir(name, ast->rdir);
}
