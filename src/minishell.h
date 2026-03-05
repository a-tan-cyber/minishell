/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:20:45 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/06 00:04:24 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "../libft/include/libft.h"
# include "minishell_struct.h"

extern volatile sig_atomic_t	g_sig;

/* 
'########:::##....##:::########:::.######.:
 ##......:::.##..##.:::##......:::##....##:
 ##......:::..####..:::##......:::##......:
 ######..:::...##...:::######..:::##......:
 ##......:::..####..:::##......:::##......:
 ##......:::.##..##.:::##......:::##....##:
 ########:::##....##:::########:::.######.:
.........:::........:::........:::::.....::
*/

//ms_exec.c
void	ms_exec_ast(t_info *i, t_ast *ast);

//ms_path.c
char	*ms_resolve_path(t_info *i, const char *cmd);

//ms_exec_utils.c
void	ms_exec_child_fail(const char *name, const char *path);
void	ms_cmd_not_found(const char *cmd);

/* 
██╗     ███████╗██╗  ██╗
██║     ██╔════╝╚██╗██╔╝
██║     █████╗   ╚███╔╝ 
██║     ██╔══╝   ██╔██╗ 
███████╗███████╗██╔╝ ██╗
╚══════╝╚══════╝╚═╝  ╚═╝
*/

//expand_dollar.c
int		expand_dollar(char **text, const char **env, t_info *info);
size_t	expand_dollar_replace(char **text, size_t i, const char **env,
			t_info *info);
char	*expand_dollar_extract(char *line, size_t *len);

// lex_expand_star.c
int		expand_star(const char *line, t_token **lexed);
t_bool	cmp_wc_rec(const char *wc, const char *thing, size_t i, size_t j);
t_bool	cmp_wc(const char *wc, const char *thing, int c);
t_token	*ins_wct(char *text, t_token **cur, t_bool *inserted);

// lex_expand.c
int		expand_tokens(t_token **lexed);
int		merge_and_expand_tokens(t_token **lexed);

//lex_main.c
size_t	tokenise_text(const char *line, t_token **lexed, const char **env,
			t_info *info);
int		lex_line(const char *line, t_token **lexed, const char **env,
			t_info *info);

// lex_tokenise_oper_rdir.c
size_t	tokenise_oper_heredoc(t_token **lexed);
size_t	tokenise_oper_rediin(t_token **lexed);
size_t	tokenise_oper_append(t_token **lexed);
size_t	tokenise_oper_rediot(t_token **lexed);

// lex_tokenise_oper.c
size_t	tokenise_oper_or(t_token **lexed);
size_t	tokenise_oper_pipe(t_token **lexed);
size_t	tokenise_oper_and(t_token **lexed);

//lex_tokenise.c
size_t	tokenise_space(const char *line, t_token **lexed);
size_t	tokenise_s_quote(const char *line, t_token **lexed);
size_t	tokenise_d_quote(const char *line, t_token **lexed, const char **env,
			t_info *info);
size_t	tokenise_oper(const char *line, t_token **lexed);
size_t	tokenise_brkt(const char *line, t_token **lexed);

// match_env.c
int		ft_envcmp(char *str, const char **arr);
char	*ft_extract_env(int i, const char **arr);
char	*match_env(char *snippet, const char **env, t_info *info);

//token_create.c
t_token	*token_create_ele(t_oper type, char *text);
t_token	*token_push_front(t_token **begin_list, t_oper type, char *text);
t_token	*token_last(t_token *begin_list);
t_token	*token_push_back(t_token **begin_list, t_oper type, char *text);
t_token	*ins_token_front(t_token **head, t_oper type, char *text);

/* 
'########:::::'###::::'########:::'######::'########:
 ##.... ##:::'## ##::: ##.... ##:'##... ##: ##.....::
 ##:::: ##::'##:. ##:: ##:::: ##: ##:::..:: ##:::::::
 ########::'##:::. ##: ########::. ######:: ######:::
 ##.....::: #########: ##.. ##::::..... ##: ##...::::
 ##:::::::: ##.... ##: ##::. ##::'##::: ##: ##:::::::
 ##:::::::: ##:::: ##: ##:::. ##:. ######:: ########:
..:::::::::..:::::..::..:::::..:::......:::........::
 */

// ast_bracket.c
int		matched_brkt(t_token *head, t_token *tail);
t_token	*skip_token_brkt_rev(t_token *tail);

// ast_create.c
t_ast	*cre_ast_node(t_type type);
t_ast	*cre_ast_new(t_token *curr);
t_redir	*crea_rdir_push_back(t_redir **curr, t_oper type, char *text);
t_ast	*cre_ast_logic(t_token *curr, t_token *head, t_token *tail);

// ast_free.c
void	free_rdir(t_redir **rdir);
void	free_ast(t_ast *ast);
void	free_ast_one(t_ast **one);

// ast_rdir.c
t_redir	*crea_rdir_node(t_oper type, char *text);
t_redir	*goto_rdir_last(t_redir *curr);

// ast_util.c
int		count_ast_arg(t_token *head, t_token *tail);
void	trim_ast_range(t_token **head, t_token **tail);

// parse_main.c
t_token	*find_token_logic_pipe(t_token *head, t_token *tail);
t_ast	*cre_ast_cmd_run(t_ast *cmd, t_token *head, t_token *tail);
t_ast	*cre_ast_cmd(t_token *head, t_token *tail);
t_ast	*build_ast_rec(t_token *head, t_token *tail);

/* 
'########::'########::::'###::::'########::
 ##.... ##: ##.....::::'## ##::: ##.... ##:
 ##:::: ##: ##::::::::'##:. ##:: ##:::: ##:
 ########:: ######:::'##:::. ##: ##:::: ##:
 ##.. ##::: ##...:::: #########: ##:::: ##:
 ##::. ##:: ##::::::: ##.... ##: ##:::: ##:
 ##:::. ##: ########: ##:::: ##: ########::
..:::::..::........::..:::::..::........:::
 */

//minishell_debug.c
void	ft_print_arr(char *name, const char **arr);
void	ft_print_rdir(char *name, t_redir *rdir);
void	print_astree(int level, t_ast *ast);

//minishell_free.c
void	free_token_one(t_token **lst);
void	free_token_lst(t_token **lst);
void	free_null_var(t_ast **ast, char ***env, char **line, t_token **lexed);
void	free_ms_var(t_ast **astree, t_info *i, char *cmd);

//minishell_init.c
void	init_ms_var(t_ast **astree, t_info *i);

//minishell_main.c
int		main(int argc, char **argv, char **envp);

//minishell_read.c
int		open_has_close(char *line, char open, char close);
t_bool	line_is_complete(char *line);

//minishell_multiline.c
char	*read_multiline(t_info *i, const char *msg);

//ms_history.c
void	ms_history_add(t_info *i, const char *line);
void	ms_history_clear(t_info *i);

//ms_quote.c
char	ms_quote_next(char q, char c);

//ms_signals.c
int		set_signals(void);
int		rl_check_sigint(void);
char	*read_multiline_sigint(char *rslt);

//ms_valid.c
t_bool	line_has_unpaired_char(char *line, char c);
t_bool	line_valid(t_info *i, char **line);

#endif