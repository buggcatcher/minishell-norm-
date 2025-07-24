
#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include "types.h"

/* String utilities */
char	*ft_strchr(char *s, int c);
char	*ft_strjoin_m(char *s1, char *s2);
char	*ft_strdup_m(char *s);
size_t	ft_strlen(char *s);
int		ft_strcmp(char *s1, char *s2);
size_t	ft_strlen_v(char *a);
char	*ft_strndup(char *s, size_t n);
char	*ft_strdup(char *s);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_append_char(char *s, char c);
char	*ft_strcpy(char *dst, const char *src);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

/* String conversion */
char	*ft_char(char *s, unsigned int number, long int len);
int		ft_nlen(int n);
char	*ft_itoa(int n);

/* Array utilities */
char	**ft_split(char *s, char c);
void	free_split_all(char **arr);
void	free_array(char **arr);

/* Memory management */
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t n_elem, size_t size);
void	*safe_alloc(size_t n_elem, size_t size, char *desc);
void	*ft_memcpy(void *dst, const void *src, size_t n);

/* Path utilities */
char	*join_path(char *dir, char *cmd);

/* Numeric utilities */
int		is_numeric(const char *str);
long	ft_strtol(const char *str, char **endptr);

/* Output utilities */
void	ft_putstr(char *str);
void	ft_putstr_stderr(char *s);

/* Cleanup utilities */
void	clean_exit(t_node *node, t_env *env, int status, t_token *token_head);
void	close_all_heredoc_fds(t_node *head);

#endif
