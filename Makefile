NAME	=		minishell
LIBFT	=		libft
SRCDIR	=		srcs
INCDIR	=		includes
OBJDIR	=		objs
CC		=		/usr/bin/clang
RM		=		/bin/rm
CFLAGS	=		-Wall -Wextra -Werror -g3 -fsanitize=address
IFLAGS	=		-I$(INCDIR) -I$(LIBFT)/includes
LFLAGS	=		-L$(LIBFT) -lft -lcurses -ltermcap

SRCS	=		$(addprefix $(SRCDIR)/,\
					$(addprefix builtins/,\
						cd.c\
						echo.c\
						env.c\
						exit.c\
						export.c\
						pwd.c\
						unset.c\
					)\
					$(addprefix bst/,\
						bst_fill.c\
						bst.c\
					)\
					$(addprefix execution/,\
						execution_fd.c\
						execution_fill.c\
						execution.c\
					)\
					$(addprefix separators/,\
						conditionals.c\
						separators.c\
					)\
					$(addprefix main/,\
						main.c\
					)\
					$(addprefix tests/,\
						tests_execution_bst.c\
					)\
					$(addprefix utils/,\
						path.c\
					)\
				)

OBJDS	=		$(addprefix $(OBJDIR)/, builtins bst execution main tests utils separators)

OBJS	=		$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

HDRS	=		$(addprefix $(INCDIR)/,\
					bst.h\
					builtins.h\
					execution.h\
					separators.h\
					path.h\
				)

all:			libft $(NAME)

libft:
	make -C $(LIBFT) NAME=libft.a CC="$(CC)" CFLAGS="$(CFLAGS)"

$(LIBFT)/libft.a: libft

$(NAME):		$(OBJDS) $(OBJS) $(LIBFT)/libft.a
	@echo LINK $(NAME)
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $(NAME)

$(OBJDS):
	mkdir -p $@

$(OBJDIR)/%.o:	$(SRCDIR)/%.c $(HDRS) Makefile
	@echo CC $<
	@$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

clean:
	make -C $(LIBFT) $@
	@echo RM $(OBJDIR)
	@$(RM) -rf $(OBJDIR)

fclean:			clean
	make -C $(LIBFT) $@
	@echo RM $(NAME)
	@$(RM) -f $(NAME)

re:				fclean all

.PHONY:			libft clean fclean

$(VERBOSE).SILENT:
