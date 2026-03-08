NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I./src -I./libft/include
LDFLAGS =
LDLIBS = -lreadline

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
READLINE_PREFIX := $(shell brew --prefix readline 2>/dev/null)
ifneq ($(READLINE_PREFIX),)
CPPFLAGS += -I$(READLINE_PREFIX)/include
LDFLAGS += -L$(READLINE_PREFIX)/lib
endif
endif

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
OBJDIR = .objs

SRC = \
	src/minishell_main.c \
	src/exec/ms_assign.c \
	src/exec/ms_assign_builtin.c \
	src/exec/ms_assign_cd.c \
	src/exec/ms_assign_exec.c \
	src/exec/ms_assign_export.c \
	src/exec/ms_brkt.c \
	src/exec/ms_builtin.c \
	src/exec/ms_builtin_basic.c \
	src/exec/ms_builtin_cd.c \
	src/exec/ms_builtin_exit.c \
	src/exec/ms_builtin_export.c \
	src/exec/ms_builtin_unset.c \
	src/exec/ms_exec.c \
	src/exec/ms_exec_cmd.c \
	src/exec/ms_exec_utils.c \
	src/exec/ms_export_print.c \
	src/exec/ms_hd_cleanup.c \
	src/exec/ms_hd_path.c \
	src/exec/ms_heredoc.c \
	src/exec/ms_path.c \
	src/exec/ms_pipe.c \
	src/exec/ms_redir.c \
	src/exec/ms_wait.c \
	src/lex/expand_dollar.c \
	src/lex/lex_expand.c \
	src/lex/lex_expand_star.c \
	src/lex/lex_main.c \
	src/lex/lex_tokenise.c \
	src/lex/lex_tokenise_oper.c \
	src/lex/lex_tokenise_oper_rdir.c \
	src/lex/lex_wc_match.c \
	src/lex/match_env.c \
	src/lex/token_create.c \
	src/parse/ast_bracket.c \
	src/parse/ast_create.c \
	src/parse/ast_free.c \
	src/parse/ast_rdir.c \
	src/parse/ast_util.c \
	src/parse/parse_main.c \
	src/read/minishell_free.c \
	src/read/minishell_init.c \
	src/read/minishell_multiline.c \
	src/read/minishell_read.c \
	src/read/ms_history.c \
	src/read/ms_quote.c \
	src/read/ms_signals.c \
	src/read/ms_valid.c \
	src/read/ms_valid_lex.c \
	src/var/ms_var_bootstrap.c \
	src/var/ms_var_edit.c \
	src/var/ms_var_list.c

DEBUG_SRC = debug/minishell_debug.c
SRCS = $(SRC)
ifeq ($(DEBUG),1)
SRCS += $(DEBUG_SRC)
endif

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) $(LDLIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

debug:
	$(MAKE) re DEBUG=1

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJDIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug

-include $(DEPS)
