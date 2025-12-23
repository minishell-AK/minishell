NAME = minishell
CC = cc
# Allow overriding CFLAGS from environment; default to empty (use -g when needed)
CFLAGS = -Wall -Wextra -Werror
SUPP_FILE = readline.supp
SRCS = $(wildcard src/*.c)
OBJ_DIR = objs
OBJS = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))



LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) -I include -L $(LIBFT_DIR) -o $(NAME) $(OBJS) -lft -lreadline

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I include -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)


valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=$(SUPP_FILE) ./$(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean || true

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean || true

re: fclean all

.PHONY: all clean fclean re
