/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:29:03 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/17 11:33:31 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_sizet(size_t *num, size_t val)
{
	*num = val;
}

char	*expand_dollar_extract(char *line, size_t *len)
{
	size_t	i;

	*len = 0;
	if (!line || !line[0] || line[0] != '$')
		return (NULL);
	i = 1;
	if (line[i] == '{')
	{
		while (line[i] && line[i] != '}')
			i++;
		if (!line[i])
			return (NULL);
		return (set_sizet(len, i + 1), ft_strndup(line + 2, i - 2));
	}
	else if (line[i] == '?')
		return (set_sizet(len, i + 1), ft_strdup("?"));
	else if (ft_isalnum(line[i]) || line[i] == '_')
	{
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		return (set_sizet(len, i), ft_strndup(line + 1, i - 1));
	}
	return (set_sizet(len, 1), ft_strndup(line, 1));
}

// return index if present, -1 if not present
int	ft_envcmp(char *str, char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		if (ft_strncmp(str, arr[i], ft_strlen(str)) == 0
			&& arr[i][ft_strlen(str)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char *ft_extract_env(int i, char **arr)
{
	int	j;
	
	j = 0;
	while (arr[i][j] && arr[i][j] != '=')
		j++;
	return (ft_strdup(arr[i] + j + 1));
}

// ?; number
char	*match_env(char *snippet, char **env, t_info *info)
{
	if (ft_strcmp(snippet, "?") == 0)
		return (ft_itoa(info->err));
	else if (ft_str_is_num(snippet) == 1)
		return (ft_strdup(""));
	else if (ft_envcmp(snippet, env) != -1)
		return (ft_extract_env(ft_envcmp(snippet, env), env));
	return (ft_strdup(""));
}

char	*ft_str_replace(char *text, size_t start, size_t len, char *expanded)
{
	size_t	total_len;
	size_t	expanded_len;
	char	*res;

	expanded_len = ft_strlen(expanded);
	total_len = ft_strlen(text) - len + expanded_len;
	res = malloc((total_len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	ft_memcpy(res, text, start);
	ft_memcpy(res + start, expanded, expanded_len);
	ft_strcpy(res + start + expanded_len, text + start + len);
	return (res);
}

size_t	expand_dollar_replace(char **text, size_t i, char **env, t_info *info)
{
	char	*snippet;
	char	*expanded;
	char	*temp;
	size_t	len;

	snippet = expand_dollar_extract(*text + i, &len);
	if (!snippet)
		return (ft_sfree(text), 0);
	if (ft_strcmp(snippet, "$") == 0)
		return (ft_sfree(&snippet), 1);
	expanded = match_env(snippet, env, info);
	ft_sfree(&snippet);
	if (!expanded)
		return (ft_sfree(text), 0);
	temp = ft_str_replace(*text, i, len, expanded);
	len = ft_strlen(expanded);
	ft_sfree(&expanded);
	ft_sfree(text);
	if (!temp)
		return (0);
	*text = temp;
	return (len);
}

int	expand_dollar(char **text, char **env, t_info *info)
{
	size_t	i;

	if (!text ||!*text)
		return (0);
	i = 0;
	while ((*text)[i])
	{
		if ((*text)[i] == '$')
		{
			i += expand_dollar_replace(text, i, env, info);
			if (!*text)
				return (0);
		}
		else
			i++;
	}
	return (1);
}

size_t	tokenise_d_quote(const char *line, t_token **lexed, const char **env,
	t_info *info)
{
	size_t	l;
	size_t	r;
	char	*text;

	l = 1;
	r = l;
	while (line[r] && line[r] != '\"')
		r++;
	if (line[r] == '\0')
		return (0);
	text = ft_strndup(line + l, r - l);
	expand_dollar(&text, env, info);
	if (!text)
		return (0);
	if (token_push_back(lexed, TEXT, text) == NULL)
		return (ft_sfree((void **)&text), 0);
	return (r + 1);
}

size_t	tokenise_space(char *line, t_token **lexed)
{
	size_t	i;

	i = 0;
	while (line[i] && ft_is_white_space(line[i]))
		i++;
	if (token_push_back(lexed, SPCE, NULL) == NULL)
		return (0);
	return (i);
}

size_t	tokenise_oper_or(t_token **lexed)
{
	if (token_push_back(lexed, OR, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_pipe(t_token **lexed)
{
	if (token_push_back(lexed, PIPE, NULL) == NULL)
		return (0);
	return (1);
}
size_t	tokenise_oper_and(t_token **lexed)
{
	if (token_push_back(lexed, AND, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_heredoc(t_token **lexed)
{
	if (token_push_back(lexed, HEREDOC, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_rediin(t_token **lexed)
{
	if (token_push_back(lexed, REDI_IN, NULL) == NULL)
		return (0);
	return (1);
}
size_t	tokenise_oper_append(t_token **lexed)
{
	if (token_push_back(lexed, APPEND, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_rediot(t_token **lexed)
{
	if (token_push_back(lexed, REDI_OT, NULL) == NULL)
		return (0);
	return (1);
}

size_t	tokenise_oper(char *line, t_token **lexed)
{
	size_t	res;

	res = 1;
	if (line[0] == '|' && line[1] == '|')
		res = tokenise_oper_or(lexed);
	else if (line[0] == '|')
		res = tokenise_oper_pipe(lexed);
	else if (line[0] == '&' && line[1] == '&')
		res = tokenise_oper_and(lexed);
	else if (line[0] == '<' && line[1] == '<')
		res = tokenise_oper_heredoc(lexed);
	else if (line[0] == '<')
		res = tokenise_oper_rediin(lexed);
	else if (line[0] == '>' && line[1] == '>')
		res = tokenise_oper_append(lexed);
	else if (line[0] == '>')
		res = tokenise_oper_rediot(lexed);
	return (res);
}

size_t	tokenise_brkt(char *line, t_token **lexed)
{
	if (line[0] == '(')
	{
		if (token_push_back(lexed, PAREN_L, NULL) == NULL)
			return (0);
		return (1);
	}
	else if (line[0] == ')')
	{
		if (token_push_back(lexed, PAREN_R, NULL) == NULL)
			return (0);
		return (1);
	}
	return (0);
}

//$
size_t	tokenise_text(char *line, t_token **lexed, char **env, t_info *info)
{
	size_t	r;
	char	*text;
	
	r = 0;
	while (line[r] && !is_operator(line[r]) && !ft_is_white_space(line[r])
			&& line[r] != '\'' && line[r] != '\"' && !is_bracket(line[r]))
		r++;
	text = ft_strndup(line, r);
	if (!text)
		return (0);
	if (expand_dollar(&text, env, info) == 0)
		return (ft_sfree((void **)&text), 0);
	ft_str_replace_chr(text, '*', 127);
	if (token_push_back(lexed, TEXT, text) == NULL)
		return (ft_sfree((void **)&text), 0);
	return (r);
}

t_bool	cmp_wc_rec(const char *wc, const char *thing, size_t i, size_t j)
{
	if (wc[i] == 127 && wc[i + 1] == '\0')
		return (TRUE);
	else if (wc[i] == '\0' && thing[j] == '\0')
		return (TRUE);
	if (wc[i] == 127)
	{
		if (cmp_wc_rec(wc, thing, i + 1, j) == TRUE)
			return (TRUE);
		if (thing[j] && cmp_wc_rec(wc, thing, i, j + 1) == TRUE)
			return (TRUE);
		return (FALSE);
	}
	if (wc[i] == thing[j])
		return (cmp_wc_rec(wc, thing, i + 1, j + 1));
	return (FALSE);
}

t_bool	cmp_wc(const char *wc, const char *thing, int c)
{
	t_bool	res;

	if (c == 127)
		res = cmp_wc_rec(wc, thing, 0, 0);
	else
		res = FALSE;
	return (res);
}

//insert a token before the current token
t_token	*ins_wct(char *text, t_token **cur, t_bool *inserted)
{
	char	*new_text;
	t_token	*new_token;

	new_text = ft_strdup(text);
	if (!new_text)
		return (ft_sfree(new_text), NULL);
	new_token = ins_token_front(cur, TEXT, new_text);
	if (new_token == NULL)
		return (NULL);
	*inserted = TRUE;
	return (new_token);
}

// expand all stars (DEL char / 127). 0 if success
// int	expand_star(char *line, t_token **lexed, char **env, t_info *info)
int	expand_star(char *line, t_token **lexed)
{
	t_bool			ins;
	DIR				*dir;
	struct dirent	*f;
	t_token			*tmp;
	
	dir = opendir(".");
	if (!dir)
		return (1);
	ins = FALSE;
	f = readdir(dir);
	while (f)
	{
		if ((((f->d_name)[0] == '.' && line[0] == '.') || f->d_name[0] != '.')
			&& cmp_wc(line, f->d_name, 127) && !ins_wct(f->d_name, lexed, &ins))
			return (closedir(dir), 2);
		f = readdir(dir);
	}
	if (ins == FALSE)
		return (ft_str_replace_chr((*lexed)->text, 127, '*'), closedir(dir), 0);
	tmp = *lexed;
	*lexed = (*lexed)->prev;
	return (free_token_one(&tmp), closedir(dir), 0);
}

int	expand_tokens(t_token **lexed)
{
	t_token	*cur;
	t_bool	is_head;

	cur = *lexed;
	while (cur)
	{
		is_head = (cur == *lexed);
		if (cur->type == TEXT && ft_strchr(cur->text, 127) &&
			expand_star(cur->text, &cur))
			return (free_token_lst(lexed), 2);
		if (is_head)
			*lexed = cur;
		cur = cur->next;
	}
	return (0);
}

// *; success returns 0, failure returns -1;
int	merge_and_expand_tokens(t_token **lexed)
{
	t_token	*cur;
	t_token	*new;
	int		err;

	cur = *lexed;
	while (cur)
	{
		if (cur->type == TEXT && cur->next && cur->next->type == TEXT)
		{
			cur->text = ft_str_append(cur->text, cur->next->text);
			if (!cur->text)
				return (free_token_lst(lexed), 1);
			free_token_one(&cur->next);
		}
		else
			cur = cur->next;
	}
	err = expand_tokens(lexed);
	return (err);
}

int	lex_line(const char *line, t_token **lexed, const char **env, t_info *info)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (line[i])
	{
		if (ft_is_white_space(line[i]))
			len = tokenise_space(line + i, lexed);
		else if (line[i] == '\'')
			len = tokenise_s_quote(line + i, lexed);
		else if (line[i] == '\"')
			len = tokenise_d_quote(line + i, lexed, env, info);
		else if (is_operator(line[i]))
			len = tokenise_oper(line + i, lexed);
		else if (is_bracket(line[i]))
			len = tokenise_brkt(line + i, lexed);
		else
			len = tokenise_text(line + i, lexed, env, info);
		if (len == 0)
			return (free_token_lst(lexed), -1);
		i += len;
	}
	return (merge_and_expand_tokens(lexed));
}

int	is_oper_token(t_oper type)
{
	if (type == PIPE || type == REDI_IN || type == REDI_OT || type == APPEND
		|| type == HEREDOC || type == AND || type == OR)
		return (1);
	return (0);
}

//skip parenthesis reverse
t_token *skip_token_brkt_rev(t_token *tail)
{
	int	r;

	if (!tail || tail->type != PAREN_R)
		return (NULL);
	r = 0;
	while (tail)
	{
		if (tail->type == PAREN_R)
			r++;
		else if (tail->type == PAREN_L)
			r--;
		tail = tail->prev;
		if (r == 0)
			return (tail);
	}
	return (NULL);
}

t_token	*find_token_logic_pipe(t_token *head, t_token *tail)
{
	t_token	*stor;

	if (!head || !tail)
		return (NULL);
	stor = tail;
	while (tail && tail != head)
	{
		if (tail->type == PAREN_R)
			tail = skip_token_brkt_rev(tail);
		else if (tail->type == AND || tail->type == OR)
			return (tail);
		else
			tail = tail->prev;
	}
	tail = stor;
	while (tail && tail != head)
	{
		if (tail->type == PAREN_R)
			tail = skip_token_brkt_rev(tail);
		else if (tail->type == PIPE)
			return (tail);
		else
			tail = tail->prev;
	}
	return (NULL);
}

t_ast	*cre_ast_node(t_type type)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (!new)
		return (NULL);
	new->type = type;
	new->args = NULL;
	new->rdir = NULL;
	new->left = NULL;
	new->riht = NULL;
	return (new);
}

t_ast	*cre_ast_new(t_token *curr)
{
	t_ast	*new;

	new = NULL;
	if (curr->type == AND)
	{
		new = cre_ast_node((t_type)AND);
	}
	else if (curr->type == OR)
	{
		new = cre_ast_node((t_type)OR);
	}
	else if (curr->type == PIPE)
	{
		new = cre_ast_node((t_type)PIPE);
	}
	if (!new)
		return (NULL);
	return (new);
}

int	matched_brkt(t_token *head, t_token *tail)
{
	int	left;

	if (head->type != PAREN_L)
		return (0);
	left = 0;
	while (TRUE)
	{
		if (head->type == PAREN_L)
			left++;
		else if (head->type == PAREN_R)
			left--;
		if (left == 0)
			break ;
		if (!head || head == tail)
			break ;
		head = head->next;
	}
	if (head == tail && left == 0)
		return (1);
	return (0);	
}

int	count_ast_arg(t_token *head, t_token *tail)
{
	int	i;

	if (!head || !tail)
		return (0);
	i = 0;
	while (TRUE)
	{
		if (head->type == TEXT)
			i++;
		if (!head || head == tail)
			break ;
		head = head->next;
	}
	return (i);
}

void	free_rdir(t_redir **rdir)
{
	t_redir	*curr;
	t_redir	*temp;

	curr = *rdir;
	while (curr)
	{
		ft_sfree(&curr->file);
		temp = curr;
		curr = curr->next;
		ft_sfree(&temp);
	}
	curr = NULL;
}

void	free_ast_one(t_ast **one)
{
	t_ast	*cur;

	cur = *one;
	cur->left = NULL;
	cur->riht = NULL;
	cur->type = NULL;
	free_arr(cur->args);
	cur->args = NULL;
	free_rdir(&cur->rdir);
	cur->rdir = NULL;
	ft_sfree(one);
}

t_redir	*crea_rdir_node(t_oper type, char *text)
{
	t_redir	*new;

	new = ft_calloc(1, sizeof(t_redir));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->type = type;
	new->file = ft_strdup(text);
	if (!new->file)
		return (ft_sfree(&new), NULL);
	return (new);
}

t_redir	*goto_rdir_last(t_redir *curr)
{
	t_redir	*temp;

	temp = curr;
	while (curr)
	{
		temp = curr;
		curr = curr->next;
	}
	return (temp);
}

t_redir	*crea_rdir_push_back(t_redir **curr, t_oper type, char *text)
{
	t_redir	*last;

	if (!curr)
		return (NULL);
	if (!*curr)
	{
		*curr = crea_rdir_node(type, text);
		if (!*curr)
			return (NULL);
		return (*curr);
	}
	last = goto_rdir_last(*curr);
	last->next = crea_rdir_node(type, text);
	if (!last->next)
		return (NULL);
	return (*curr);
}

t_ast	*cre_ast_cmd_run(t_ast *cmd, t_token *head, t_token *tail)
{
	int	i;

	i = 0;
	while (head)
	{
		if (head->type == TEXT)
		{
			cmd->args[i] = ft_strdup(head->text);
			if (!cmd->args[i])
				return (free_ast_one(&cmd), NULL);
			i++;
		}
		else
		{
			if (head->next &&
				!crea_rdir_push_back(&cmd->rdir,head->type, head->next->text))
				return (free_ast_one(&cmd), NULL);
			head = head->next;
		}
		if (head == tail)
			break ;
		head = head->next;
	}
	return (cmd);
}

t_ast	*cre_ast_cmd(t_token *head, t_token *tail)
{
	t_ast	*cmd;
	int		i;

	cmd = cre_ast_node(CMD);
	if (!cmd)
		return (NULL);
	i = count_ast_arg(head, tail);
	cmd->args = ft_calloc(i + 1, sizeof(char *));
	cmd = cre_ast_cmd_run(cmd, head, tail);
	if (!cmd)
		return (NULL);
	return (cmd);
}

t_ast	*build_ast_rec(t_token *head, t_token *tail)
{
	t_ast	*new;
	t_token	*curr;

	if (!head || !tail)
		return (NULL);
	curr = find_token_logic_pipe(head, tail);
	if (curr)
	{
		new = cre_ast_new(curr);
		if (!new)
			return (NULL);
		new->left = build_ast_rec(head, curr->prev);
		new->riht = build_ast_rec(curr->next, tail);
		return (new);
	}
	if (head->type == PAREN_L && tail->type == PAREN_R && matched_brkt(head, tail))
	{
		new = cre_ast_node((t_type)BRKT);
		if (!new)
			return (NULL);
		new->left = build_ast_rec(head->next, tail->prev);
		return (new);
	}
	return (cre_ast_cmd(head, tail));
}

t_ast	*parse_ast(t_token *head)
{
	t_token	*curr;
	t_ast	*root;

	curr = split_ast_by_logic(head);
	while (curr)
	{
		ins_ast_left(&root, curr);
		curr = split_ast_by_logic(head);
	}
	curr = split_ast_by_pipe(head);
	while (curr)
	{
		ins_ast_left(&root, curr);
		curr = split_ast_by_pipe(head);
	}
}

// print_astree(astree)

int	main(int argc, char **argv, char **envp)
{
	t_ast	*astree;
	t_info	i;

	init_ms_var(&astree, &i);
	signal(SIGINT, int_handler);
	signal(SIGQUIT, SIG_IGN);
	if (argc != 1 || ft_arrdup_mk2(&i.my_env, envp) != 0)
		return (free_ms_var(&astree, &i, "all"), 2);
	while (TRUE)
	{
		i.line = read_multiline("moonshell> ");
		if (!i.line || !ft_strcmp(i.line, "exit\n"))
			return (free_ms_var(&astree, &i, "all"), 0);
		if (line_valid(&i.line))
		{
			add_history(i.line);
			if (lex_line(i.line, &i.lexed, i.my_env, &i) == 0)
				astree = parse_ast(i.lexed);
			if (astree)
				print_astree(astree);
				// run_cmd(astree, i);
		}
		free_ms_var(&astree, &i, "tmp");
	}
	return (free_ms_var(&astree, &i, "all"), 0);
}
// int	main(int argc, char **argv, char **envp)
// {
// 	t_ast	*astree;
// 	char	**my_env;
// 	char	*line;
// 	char	*lexed;

// 	nullise_var(&astree, &my_env, &line, &lexed);
// 	if (argc != 1 || ft_arrdup_mk2(&my_env, envp) != 0)
// 		return (free_null_var(NULL, &my_env, NULL, NULL), 2);
// 	while (TRUE)
// 	{
// 		line = read_multiline("moonshell> ");
// 		//if line has error then freenullvar then exit.
// 		if (!line || ft_strcmp(line, "exit") == 0)
// 			return (free_null_var(&astree, &my_env, &line, &lexed), 0);
// 		if (line != NULL)
// 			add_history(line);
// 		if (line && lex_line(line, &lexed) == 0)
// 		{
// 			astree = build_ast(lexed);
// 			if (astree)
// 				run_cmd(astree);
// 		}
// 		free_null_var(&astree, NULL, &line, &lexed);
// 	}
// 	return (free_null_var(&astree, &my_env, NULL, NULL), 0);
// }
