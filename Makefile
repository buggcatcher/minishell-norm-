NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude
SRCDIR = src

VPATH = $(SRCDIR)/btin:$(SRCDIR)/core:$(SRCDIR)/debug:$(SRCDIR)/env:$(SRCDIR)/exe:$(SRCDIR)/hdoc:$(SRCDIR)/libfx:$(SRCDIR)/memory:$(SRCDIR)/parse:$(SRCDIR)/signals

all: $(NAME)

BTIN_SRCS = $(SRCDIR)/btin/btin_apply.c $(SRCDIR)/btin/btin_search.c \
			$(SRCDIR)/btin/exe_btin.c $(SRCDIR)/btin/exe_cd.c \
			$(SRCDIR)/btin/exe_exit.c $(SRCDIR)/btin/exe_export.c \
			$(SRCDIR)/btin/exe_export_parse.c $(SRCDIR)/btin/exe_export_print.c

CORE_SRCS = $(SRCDIR)/core/main.c $(SRCDIR)/core/executor_loop.c \
			$(SRCDIR)/core/child.c

ENV_SRCS = $(SRCDIR)/env/env.c $(SRCDIR)/env/system_env.c $(SRCDIR)/env/various.c

EXE_SRCS = $(SRCDIR)/exe/redirects.c $(SRCDIR)/exe/resolve_path.c \
		   $(SRCDIR)/exe/fd.c $(SRCDIR)/exe/path.c $(SRCDIR)/exe/parent_exe.c 

HDOC_SRCS = $(SRCDIR)/hdoc/heredoc_buffer.c $(SRCDIR)/hdoc/heredoc_preprocessor.c \
			$(SRCDIR)/hdoc/read_heredoc.c

LIBFX_SRCS = $(SRCDIR)/libfx/libfx.c $(SRCDIR)/libfx/utils.c \
			 $(SRCDIR)/libfx/utils_mini.c $(SRCDIR)/libfx/itoa_mini.c \
			 $(SRCDIR)/libfx/split.c $(SRCDIR)/libfx/libfx_mini.c

MEMORY_SRCS = $(SRCDIR)/memory/memory.c $(SRCDIR)/memory/free_mini.c \
			  $(SRCDIR)/memory/free.c

PARSE_SRCS = $(SRCDIR)/parse/tokenize_mini.c $(SRCDIR)/parse/token_type_mini.c \
			 $(SRCDIR)/parse/dollar_mini.c $(SRCDIR)/parse/syntax_check_mini.c \
			 $(SRCDIR)/parse/node_mini.c $(SRCDIR)/parse/node_argv_mini.c \
			 $(SRCDIR)/parse/dummy_nodes.c $(SRCDIR)/parse/dummy_placeholder.c \
			 $(SRCDIR)/parse/node_argv.c $(SRCDIR)/parse/dollar.c \
			 $(SRCDIR)/parse/node.c $(SRCDIR)/parse/token_type_quotes.c \
			 $(SRCDIR)/parse/token_type.c


SIGNALS_SRCS = $(SRCDIR)/signals/signals.c

SRCS = $(BTIN_SRCS) $(CORE_SRCS) $(DEBUG_SRCS) $(ENV_SRCS) $(EXE_SRCS) \
	   $(HDOC_SRCS) $(LIBFX_SRCS) $(MEMORY_SRCS) $(PARSE_SRCS) $(SIGNALS_SRCS)

OBJS = $(notdir $(SRCS:.c=.o))

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline
	@rm -f $(OBJS)   
	@echo "Eseguibile $(NAME) creato con successo!"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@echo "Pulizia filename oggetto completata!"

fclean: clean
	@rm -f $(NAME)
	@echo "Pulizia completa (incluso $(NAME))!"

re: fclean all

val: all
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	--suppressions=readline.supp --track-fds=yes --quiet ./$(NAME)

.PHONY: all clean fclean re
