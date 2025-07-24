
#ifndef MINISHELL_H
# define MINISHELL_H

/* System includes */
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
# include <ctype.h>

/* Project includes */
# include "types.h"
# include "parser.h"
# include "executor.h"
# include "builtins.h"
# include "environment.h"
# include "utils.h"
# include "signals.h"
# include "heredoc.h"

/* Constants */
# define PATH_MAX 4096

/* Global variables */
extern volatile sig_atomic_t	g_sigint;

#endif
