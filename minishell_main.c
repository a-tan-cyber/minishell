#include <readline/readline.h>
#include <readline/history.h>

typedef int				t_bool;
# define TRUE 1
# define FALSE 0

enum	e_type
{
	cmd,
	pipe,
	or,
	and,
	rdir,
	brkt,
};

typedef enum e_type		t_type;

struct	s_ast
{
	enum e_type		type;
	char			*data;
	struct s_ast	*left;
	struct s_ast	*right;
};

typedef struct s_ast	t_ast;

int	main(int argc, char **argv, char **envp)
{
	t_ast	*astree;
	char	**my_env;
	char	*line;
	char	*parsed;

	nullise_var(&astree, &my_env, &line, &parsed);
	if (argc != 1 || init_env(&my_env, envp) != 0)
		return (clearall(NULL, &my_env, NULL, NULL), 2);
	while (TRUE)
	{
		line = read_multiline("moonshell> ");
		if (!line || ft_strcmp(line, "exit") == 0)
			return (clearall(&astree, &my_env, &line, &parsed), 0);
		if (line != NULL)
			add_history(line);
		if (line && parse_line(line, &parsed) == 0)
		{
			astree = build_ast(parsed);
			if (astree)
				run_cmd(astree);
		}
		clearall(&astree, NULL, &line, &parsed);
	}
	return (clearall(&astree, &my_env, NULL, NULL), 0);
}
