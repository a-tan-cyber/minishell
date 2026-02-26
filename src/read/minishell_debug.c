/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_debug.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:14:20 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/26 22:41:22 by amtan            ###   ########.fr       */
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
		printf("%d", (int)rdir->type);
		rdir = rdir->next;
	}
	printf("}\n");
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
	ft_strcpy(name, "args lvl ");
	ft_itoa_fast(level, name + 9);
	ft_print_arr(name, (const char **)ast->args);
	ft_strcpy(name, "rdir lvl ");
	ft_itoa_fast(level, name + 9);
	ft_print_rdir(name, ast->rdir);
}
