/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:20:45 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/13 11:55:45 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/include/libft.h"
# include "minishell_struct.h"

//minishell_debug.c

//minishell_free.c
void	free_ast(t_ast *ast);
void	free_null_var(t_ast **ast, char ***env, char **line, char **parsed);

//minishell_init.c

//minishell_read.c
char	*read_multiline(char *msg);

#endif