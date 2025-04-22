NAME    = minishell

# Compiler + Flags
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
INCS    = -Iincludes

# Gather all .c files from subfolders
SRC     = $(wildcard src/builtins/*.c) \
          $(wildcard src/executable/*.c) \
          $(wildcard src/libft/*.c) \
          $(wildcard src/main/*.c) \
          $(wildcard src/parsing/*.c) \
          $(wildcard src/pipes/*.c) \
          $(wildcard src/redirections/*.c)

# Convert .c -> .o
OBJ     = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lreadline -lhistory -lncurses -o $(NAME)

# Generic .o rule
%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
