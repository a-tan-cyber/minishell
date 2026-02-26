#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t	g_sig;

static void	handle_sigint(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	write(1, "^C", 2);
}

static int	rl_check_sigint(void)
{
	if (g_sig == SIGINT)
	{
		rl_replace_line("", 1);
		rl_done = 1;
	}
	return (0);
}

static void	set_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

int	main(void)
{
	char	*line;

	g_sig = 0;
	rl_catch_signals = 0;
	rl_event_hook = rl_check_sigint;
	set_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (g_sig == SIGINT)
		{
			g_sig = 0;
			free(line);
			continue ;
		}
		if (line[0] != '\0')
			add_history(line);
		free(line);
	}
	rl_clear_history();
	write(1, "exit\n", 5);
	return (0);
}
