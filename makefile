# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -I./includes

# Executable name
NAME = minishell

# Source and object files
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Libraries (add -lreadline if using readline library)
LIBS = -lreadline

# Default target to build the executable
all: $(NAME)

# Rule to build the executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

# Rule to compile .c files to .o files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files
clean:
	rm -f $(OBJS)

# Clean up everything, including the executable
fclean: clean
	rm -f $(NAME)

# Rebuild the project
re: fclean all
