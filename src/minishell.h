/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:20:45 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/08 22:01:52 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <errno.h>
# include <limits.h>
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
int		ms_exec_cmd_path(t_info *i, t_ast *cmd, const char *name,
			const char *path);

//ms_exec_cmd.c
void	ms_exec_cmd(t_info *i, t_ast *ast);

//ms_path.c
char	*ms_resolve_path(t_info *i, const char *cmd);

//ms_builtin.c
int		ms_try_builtin(t_info *i, t_ast *cmd);

//ms_builtin_basic.c
int		ms_builtin_echo(char **argv);
int		ms_builtin_pwd(void);
int		ms_builtin_env(t_info *i);
int		ms_builtin_unset(t_info *i, char **argv);

//ms_builtin_cd.c
int		ms_builtin_cd(t_info *i, char **argv);

//ms_builtin_exit.c
int		ms_builtin_exit(t_info *i, char **argv);

//ms_builtin_export.c
int		ms_builtin_export(t_info *i, char **argv);

//ms_assign.c
int		ms_assign_ident(const char *arg, size_t *eq);
int		ms_assign_apply_n(t_var **vars, char **argv, int n, t_bool exported);
int		ms_assign_prefix_len(char **argv);
int		ms_try_assign_only(t_info *i, t_ast *cmd);

//ms_assign_exec.c
int		ms_try_assign_exec(t_info *i, t_ast *cmd);

//ms_assign_builtin.c
int		ms_try_assign_builtin(t_info *i, t_ast *cmd);

//ms_assign_cd.c
int		ms_try_assign_cd(t_info *i, t_ast *cmd);

//ms_assign_export.c
int		ms_try_assign_export(t_info *i, t_ast *cmd);

//ms_brkt.c
int		ms_exec_brkt(t_info *i, t_ast *ast);

//ms_redir.c
int		ms_redir_apply(t_redir *rdir);
int		ms_redir_parent_begin(t_redir *rdir, int *save_in, int *save_out);
void	ms_redir_parent_end(int save_in, int save_out);

//ms_heredoc.c
int		ms_heredoc_prepare_ast(t_info *i, t_ast *ast);

//ms_hd_path.c
char	*ms_hd_path(int idx);
t_bool	ms_hd_is_delim(char *line, const char *delim);
int		ms_hd_write_line(t_info *i, int fd, char *line, t_bool expand);
int		ms_hd_fail_one(t_info *i, char **path, int fd);

//ms_hd_cleanup.c
void	ms_hd_warn_eof(t_redir *r);
void	ms_heredoc_cleanup_ast(t_ast *ast);

//ms_pipe.c
int		ms_exec_pipe(t_info *i, t_ast *ast);

//ms_export_print.c
void	ms_export_print_all(t_var *vars);

//ms_wait.c
int		set_wait_signals(void);
void	set_child_signals(void);
int		ms_wait_pid(pid_t pid, int *status, int options);
int		ms_wait_status(int status, t_bool interactive, t_bool print_msg);

//ms_exec_utils.c
void	ms_exec_child_fail(const char *name, const char *path);
void	ms_cmd_not_found(const char *cmd);
int		ms_restore_signals_ret(t_info *i, int ret);
int		ms_exec_redir_only(t_ast *ast);

/* 
'##::::'##::::'###::::'########::
 ##::::'##:::'## ##::: ##.... ##:
 ##::::'##::'##:. ##:: ##:::: ##:
 ##::::'##:'##:::. ##: ########::
. ##::'##:: #########: ##.. ##:::
:. ##'##::: ##.... ##: ##::. ##::
::. ###:::: ##:::: ##: ##:::. ##:
:::...:::::..:::::..::..:::::..::
*/

//ms_var_list.c
t_var	*ms_var_new(const char *name, const char *value, t_bool exported);
void	ms_var_add_back(t_var **lst, t_var *node);
void	ms_var_clear(t_var **lst);
t_var	*ms_var_dup(t_var *vars);

//ms_var_bootstrap.c 
char	**ms_var_build_env(t_var *vars);
int		ms_var_bootstrap(t_info *i, char **envp);
char	*ms_var_get(t_var *vars, const char *name);
int		ms_var_set(t_var **vars, const char *name, const char *value,
			t_bool exported);
void	ms_var_unset(t_var **vars, const char *name);
int		ms_var_sync_env(t_info *i);

/* 
██╗     ███████╗██╗  ██╗
██║     ██╔════╝╚██╗██╔╝
██║     █████╗   ╚███╔╝ 
██║     ██╔══╝   ██╔██╗ 
███████╗███████╗██╔╝ ██╗
╚══════╝╚══════╝╚═╝  ╚═╝
*/

//expand_dollar.c
int		expand_dollar(char **text, t_info *info);

//lex_expand_star.c
int		expand_star(const char *line, t_token **lexed);

//lex_wc_match.c
t_bool	cmp_wc(const char *wc, const char *thing, int c);

// lex_expand.c
int		merge_and_expand_tokens(t_token **lexed);

//lex_main.c
int		lex_line(const char *line, t_token **lexed, t_info *info);

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
size_t	tokenise_s_quote(const char *line, t_token **lexed, t_bool hd_delim);
size_t	tokenise_d_quote(const char *line, t_token **lexed, t_info *info,
			t_bool hd_delim);
size_t	tokenise_oper(const char *line, t_token **lexed);
size_t	tokenise_brkt(const char *line, t_token **lexed);

// match_env.c
char	*match_env(char *snippet, t_info *info);

//token_create.c
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
t_redir	*crea_rdir_push_back(t_redir **curr, t_token *oper, t_token *file);
t_ast	*cre_ast_logic(t_token *curr, t_token *head, t_token *tail);

// ast_free.c
void	free_ast(t_ast *ast);
void	free_ast_one(t_ast **one);

// ast_rdir.c
t_redir	*crea_rdir_node(t_token *oper, t_token *file);
t_redir	*goto_rdir_last(t_redir *curr);

// ast_util.c
int		count_ast_arg(t_token *head, t_token *tail);
void	trim_ast_range(t_token **head, t_token **tail);

// parse_main.c
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

//minishell_free.c
void	free_token_one(t_token **lst);
void	free_token_lst(t_token **lst);
void	free_ms_var(t_ast **astree, t_info *i, char *cmd);

//minishell_init.c
void	init_ms_var(t_ast **astree, t_info *i);

//minishell_read.c
int		open_has_close(char *line, char open, char close);
t_bool	line_is_complete(char *line);

//minishell_multiline.c
char	*read_multiline(t_info *i, const char *msg);

//ms_history.c
void	ms_history_add(t_info *i, const char *line);
void	ms_history_clear(t_info *i);

//ms_quote.c
char	*ms_input_next_line(t_info *i, const char *prompt);
char	ms_quote_next(char q, char c);

//ms_signals.c
int		set_signals(void);
int		rl_check_sigint(void);
char	*read_multiline_sigint(char *rslt);

//ms_valid.c
t_bool	line_valid(t_info *i, char **line);

/* 
'########:::'########::'########:::'##::::'##:::..######:::
 ##.... ##:: ##.....::: ##.... ##:: ##:::: ##:::.##... ##::
 ##:::: ##:: ##:::::::: ##:::: ##:: ##:::: ##:::##:::::::::
 ##:::: ##:: ######:::: ########::: ##:::: ##:::##:: ####::
 ##:::: ##:: ##...::::: ##.... ##:: ##:::: ##:::##:::: ##::
 ##:::: ##:: ##:::::::: ##:::: ##:: ##:::: ##:::##:::: ##::
 ########::: ########:: ########:::. #######::::. #######::
 .......::::........:::........:::::.......::::::........::
debug ast by: 
- uncommenting print_astree prototype below 
- inserting:
		print_astree(0, *ast); 
	as the last line of ms_process_line in minishell_main.c
- make debug version of the shell with `make debug`
- run ./minishell then enter a command to see the ast printed in the terminal
*/

//minishell_debug.c
// void	print_astree(int level, t_ast *ast);

#endif
