
#ifndef SIGNALS_H
# define SIGNALS_H

/* Signal handling functions */
void	handle_sigint(int sig);
void	setup_signals(void);
void	disable_signals(void);
void	ignore_signals(void);

#endif
