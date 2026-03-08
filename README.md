# Minishell

*This project has been created as part of the 42 curriculum by yunguo and amtan.*

## Description

Minishell is a 42 core curriculum project ([https://www.42network.org](https://www.42network.org)) in which students build a small Unix shell in C. The objective of the project is to reproduce the core behavior of Bash closely enough to understand how a real shell works under the hood: reading user input, tokenizing and parsing commands, expanding variables, handling redirections and pipes, launching processes, and managing signals.

This project is mainly about **processes, file descriptors, parsing, and shell behavior**. Instead of calling `system()` or relying on an existing shell, the program builds its own execution flow using low-level functions such as `fork`, `execve`, `pipe`, `dup2`, `waitpid`, and `signal`.

In this codebase, the shell is split into clear stages:

* **Read** user input with GNU Readline and keep command history.
* **Lex** the input into tokens while handling quotes, operators, and expansions.
* **Parse** tokens into an AST (abstract syntax tree).
* **Execute** builtins, external commands, redirections, pipelines, and grouped expressions.
* **Manage** environment variables, exit status, heredocs, and interactive signals.

### What this implementation supports 

* interactive prompt and history
* execution of commands through `PATH`, relative paths, and absolute paths
* built-ins:

  * `echo` with option `-n`
  * `cd` with only a relative or absolute path
  * `pwd` with no options
  * `export` with no options
  * `unset` with no options
  * `env` with no options or arguments
  * `exit` with no options
* single quotes and double quotes
* environment variable expansion, including `$?`
* redirections: `<`, `>`, `<<`, `>>`
* pipelines with `|`
* signal handling for interactive use (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`)
* heredocs
* logical operators `&&` and `||`
* parentheses for grouped execution
* wildcard expansion with `*` in the current working directory

### Project Constraints

* **Allowed external functions:** `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`, `printf`, `malloc`, `free`, `write`, `access`, `open`, `read`, `close`, `fork`, `wait`, `waitpid`, `wait3`, `wait4`, `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`, `exit`, `getcwd`, `chdir`, `stat`, `lstat`, `fstat`, `unlink`, `execve`, `dup`, `dup2`, `pipe`, `opendir`, `readdir`, `closedir`, `strerror`, `perror`, `isatty`, `ttyname`, `ttyslot`, `ioctl`, `getenv`, `tcsetattr`, `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`
* **Global variables:** at most one global variable to indicate a received signal
* **Unsupported special characters:** the shell does not interpret special characters such as `\` and `;`

### Project Structure

```text
src/
├── exec/             # execution, builtins, pipes, redirections, heredocs
├── lex/              # tokenization and expansions
├── parse/            # AST construction
├── read/             # readline input, history, validation, signals
├── var/              # environment variable storage and synchronization
├── minishell.h
├── minishell_struct.h
└── minishell_main.c  # program entry point

debug/                # debugging helpers / test utilities
libft/                # project utility library
Makefile
README.md
```

## Instructions

This project is written in C and uses **GNU Readline** for line editing and history.

### Prerequisites

You need:

* `cc`
* `make`
* the GNU Readline headers and library

Examples:

**Ubuntu / Debian**

```bash
sudo apt update
sudo apt install build-essential libreadline-dev
```

**macOS (Homebrew)**

```bash
brew install readline
```

> Note: the provided `Makefile` already tries to detect the Homebrew Readline prefix on macOS.

### Compilation

To compile the project with the required flags `-Wall`, `-Wextra`, and `-Werror`:

```bash
make
```

### Execution

```bash
./minishell
```

The current prompt string in this codebase is:

```text
moonshell>
```

### Example session

```bash
moonshell> echo "Hello from moonshell!"
Hello from moonshell!
moonshell> export NAME=42
moonshell> echo $NAME
42
moonshell> cd src/
moonshell> ls
exec  minishell.h       minishell_struct.h  read
lex   minishell_main.c  parse               var
moonshell> ls | wc -l
8
moonshell> cat << EOF
> heredoc example
> EOF
heredoc example
moonshell> exit
exit
```

## Tester Compatibility Note

If you plan to validate this project with the LucasKuhn minishell tester ([https://github.com/LucasKuhn/minishell_tester/tree/main](https://github.com/LucasKuhn/minishell_tester/tree/main)), apply the following minimal patch to the tester script first:

```diff
--- a/tester
+++ b/tester
@@
 REMOVE_COLORS="sed s/\x1B\[[0-9;]\{1,\}[A-Za-z]//g"
 REMOVE_EXIT="grep -v ^exit$"
+
+strip_prompt()
+{
+	if [[ -n "$PROMPT" ]]; then
+		grep -vF -- "$PROMPT"
+	else
+		cat
+	fi
+}
@@
-		MINI_OUTPUT=$(echo -e "$teste" | $MINISHELL_PATH 2> /dev/null | $REMOVE_COLORS | grep -vF "$PROMPT" | $REMOVE_EXIT )
+		MINI_OUTPUT=$(echo -e "$teste" | $MINISHELL_PATH 2> /dev/null | $REMOVE_COLORS | strip_prompt | $REMOVE_EXIT )
@@
-		MINI_EXIT_CODE=$(echo -e "$MINISHELL_PATH\n$teste\necho \$?\nexit\n" | bash 2> /dev/null | $REMOVE_COLORS | grep -vF "$PROMPT" | $REMOVE_EXIT | tail -n 1)
+		MINI_EXIT_CODE=$(echo -e "$MINISHELL_PATH\n$teste\necho \$?\nexit\n" | bash 2> /dev/null | $REMOVE_COLORS | strip_prompt | $REMOVE_EXIT | tail -n 1)
```

This patch is needed because this minishell also supports **non-interactive mode**, like Bash. In non-interactive mode, the shell correctly produces **no prompt**, but the tester assumes it can always detect one and then remove it from later output. If the detected prompt is empty, the tester ends up running `grep -vF "$PROMPT"` with an empty pattern, which removes every line of output instead of only prompt lines. The `strip_prompt` helper fixes that by filtering prompt lines only when a non-empty prompt was actually detected.

## Resources

Useful references for understanding the ideas behind this project:

* **GNU Bash Reference Manual** — shell behavior, quoting, expansions, pipelines, and redirections: [https://www.gnu.org/s/bash/manual/bash.html](https://www.gnu.org/s/bash/manual/bash.html)
* **GNU Readline documentation** — interactive input editing and history: [https://www.gnu.org/s/readline/](https://www.gnu.org/s/readline/)
* **POSIX Shell Command Language** — standard shell grammar and behavior: [https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html)
* **Linux man pages** — especially `fork(2)`, `execve(2)`, `pipe(2)`, `dup2(2)`, `waitpid(2)`, and `signal(7)`: [https://man7.org/linux/man-pages/](https://man7.org/linux/man-pages/)

### AI Usage

AI was used for learning, code review, debugging guidance, and documentation. It helped explain shell concepts, review inherited code, discuss implementation ideas, and polish this README. All final design, integration, testing, and validation were done manually against the 42 minishell subject and evaluation expectations.

##
