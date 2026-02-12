/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:20:45 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/12 16:02:52 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/types.h>
# include "../libft/include/libft.h"
# include "minishell_struct.h"

volatile sig_atomic_t g_sig = 0;

//lex_main.c
size_t	skip_whitespace(const char *line);
int		lex_line(const char *line, t_token **lexed);

//lex_tokenise.c
size_t	tokenise_s_quote(const char *line, t_token **lexed);

//token_create.c
t_token	*token_create_ele(t_oper type, char *text);
t_token	*token_push_front(t_token **begin_list, t_oper type, char *text);
t_token	*token_last(t_token *begin_list);
t_token	*token_push_back(t_token **begin_list, t_oper type, char *text);
t_token	*ins_token_front(t_token **head, t_oper type, char *text);

//minishell_debug.c

//minishell_free.c
void	free_ast(t_ast *ast);
void	free_token_one(t_token **lst);
void	free_token_lst(t_token **lst);
void	free_null_var(t_ast **ast, char ***env, char **line, t_token **lexed);
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

//ms_valid.c
t_bool	line_valid(char **line);

#endif