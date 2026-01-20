/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:20:45 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/21 06:17:36 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "../libft/include/libft.h"
# include "minishell_struct.h"

volatile sig_atomic_t g_sig = 0;

//minishell_debug.c

//minishell_free.c
void	free_ast(t_ast *ast);
void	free_null_var(t_ast **ast, char ***env, char **line, char **parsed);
void	free_ms_var(t_ast **astree, t_info *i, char *cmd);

//minishell_init.c
int		init_ms_var(t_ast **astree, t_info *i);

//minishell_read.c
t_bool	open_has_close(char *line, char open, char close);
t_bool	line_is_complete(char *line);
char	*read_multiline(char *msg);

//ms_signals.c
void	int_handler(int sig);
char	*read_multiline_sigint(char *rslt);


#endif