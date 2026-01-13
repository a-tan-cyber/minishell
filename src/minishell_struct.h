/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_struct.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:20:24 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/13 08:29:21 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef int				t_bool;
# define TRUE 1
# define FALSE 0

enum	e_type
{
	CMD,
	PIPE,
	OR,
	AND,
	RDIR,
	BRKT
};

typedef enum e_type		t_type;

//riht means right in ye olde English 🧐
struct	s_ast
{
	enum e_type		type;
	char			*data;
	struct s_ast	*left;
	struct s_ast	*riht;
};

typedef struct s_ast	t_ast;