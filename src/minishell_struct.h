/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_struct.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:20:24 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/06 15:22:56 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCT_H
# define MINISHELL_STRUCT_H

# include <stddef.h>

typedef int				t_bool;
# define TRUE 1
# define FALSE 0

enum	e_type
{
	AST_CMD,
	AST_PIPE,
	AST_OR,
	AST_AND,
	// AST_RDIR,
	AST_BRKT
};

typedef enum e_type		t_type;

// TEXT: "", ''; PIPE: |; REDI_IN: <; REDI_OT: >; APPEND: >>; HEREDOC: <<; 
// AND: &&; OR: ||; PAREN_L: (; PAREN_R: )
enum	e_oper
{
	SPCE,
	TEXT,
	PIPE,
	REDI_IN,
	REDI_OT,
	APPEND,
	HEREDOC,
	AND,
	OR,
	PAREN_L,
	PAREN_R
};

typedef enum e_oper		t_oper;

// REDI_IN: <; REDI_OT: >; APPEND: >>; HEREDOC: <<; 
struct	s_redir
{
	enum e_oper		type;
	char			*file;
	t_bool			is_hd_tmp;
	struct s_redir	*next;
};

typedef struct s_redir	t_redir;

//riht means right in ye olde English 🧐
struct	s_ast
{
	enum e_type		type;
	char			**args;
	t_redir			*rdir;
	struct s_ast	*left;
	struct s_ast	*riht;
};

typedef struct s_ast	t_ast;

struct	s_hist
{
	char			*line;
	struct s_hist	*next;
};

typedef struct s_hist	t_hist;

typedef struct s_var
{
	char			*name;
	char			*value;
	t_bool			exported;
	struct s_var	*next;
}	t_var;

struct	s_info
{
	t_bool			interactive;
	t_hist			*hist;
	char			**my_env;
	t_var			*vars;
	char			*line;
	int				err;
	struct s_token	*lexed;
	struct s_ast	*ast;
	t_bool			exit_req;
};

typedef struct s_info	t_info;

struct	s_token
{
	enum e_oper		type;
	char			*text;
	struct s_token	*prev;
	struct s_token	*next;
};

typedef struct s_token	t_token;

typedef struct s_wc_ctx
{
	const char	*wc;
	const char	*th;
	signed char	*memo;
	size_t		lt;
}	t_wc_ctx;

typedef struct s_pathctx
{
	const char	*path;
	const char	*cmd;
	char		*fallback;
}	t_pathctx;

typedef struct s_exitnum
{
	const char			*s;
	size_t				i;
	int					sign;
	unsigned long long	acc;
	unsigned long long	lim;
}	t_exitnum;

#endif