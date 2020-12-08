#ifndef PATH_H
# define PATH_H

# include <limits.h>
# include <stdlib.h>
# include <sys/stat.h>

# include <libft.h>

char	*path_cat(const char *a, const char *b);
char	*path_get(const char *name, const char *path);

#endif
