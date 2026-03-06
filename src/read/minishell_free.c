/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 09:57:27 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/06 19:21:28 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_token_one(t_token **lst)
{
	t_token	*node;
	t_token	*next;

	if (!lst || !*lst)
		return ;
	node = *lst;
	next = node->next;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	*lst = next;
	ft_sfree((void **)&node->text);
	ft_sfree((void **)&node);
}

void	free_token_lst(t_token **lst)
{
	while (lst && *lst)
		free_token_one(lst);
}

static void	free_null_var(t_ast **ast, char ***env, char **line,
							t_token **lexed)
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
		ft_sfree((void **)line);
		*line = NULL;
	}
	if (lexed != NULL)
	{
		free_token_lst(lexed);
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
		ms_var_clear(&i->vars);
		ms_history_clear(i);
	}
}
