/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_struct.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:20:24 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/01 13:26:12 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCT_H
# define MINISHELL_STRUCT_H

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

struct	s_info
{
	t_bool			interactive;
	t_hist			*hist;
	char			**my_env;
	char			*line;
	int				err;
	struct s_token	*lexed;
	struct s_ast	*ast;
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

#endif