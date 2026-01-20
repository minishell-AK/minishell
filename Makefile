NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIE
SUPP_FILE = readline.supp

SRCS = \
	src/add_variables.c \
	src/add_variables_copy.c \
	src/add_variables_free.c \
	src/add_variables_path.c \
	src/cleanup.c \
	src/env_utils.c \
	src/exec/add_pipe.c \
	src/exec/builtin/ft_cd.c \
	src/exec/builtin/ft_cd_helpers.c \
	src/exec/builtin/ft_echo.c \
	src/exec/builtin/ft_env.c \
	src/exec/builtin/ft_exit.c \
	src/exec/builtin/ft_export.c \
	src/exec/builtin/ft_export_print.c \
	src/exec/builtin/ft_export_set.c \
	src/exec/builtin/ft_pwd.c \
	src/exec/builtin/ft_unset.c \
	src/exec/builtin/is_builtin.c \
	src/exec/close_pipe.c \
	src/exec/exe_cmd.c \
	src/exec/exe_cmd_helpers.c \
	src/exec/exe_cmd_spawn.c \
	src/exec/find_path.c \
	src/exec/open_file.c \
	src/exec/setup_child.c \
	src/expander.c \
	src/expander_core.c \
	src/expander_core_helpers.c \
	src/expander_env.c \
	src/expander_utils.c \
	src/free_all_variables.c \
	src/lexer.c \
	src/lexer_helpers.c \
	src/lexer_word_build.c \
	src/lexer_word.c \
	src/lexer_word_finalize.c \
	src/lexer_word_finalize_helpers.c \
	src/lexer_word_flags.c \
	src/lexer_word_seg.c \
	src/lexer_word_utils.c \
	src/main.c \
	src/main_helpers.c \
	src/main_signals.c \
	src/main_state.c \
	src/parser.c \
	src/parser_cmd.c \
	src/parser_cmd_helpers.c \
	src/parser_helpers.c \
	src/parser_helpers_utils.c \
	src/parser_heredoc.c \
	src/parser_heredoc_io.c \
	src/parser_heredoc_spawn.c \
	src/parser_heredoc_helpers.c \
	src/parser_heredoc_tmp.c \
	src/parser_heredoc_utils.c \
	src/parser_heredoc_wait.c \
	src/parser_redir.c \
	src/parser_utils.c \
	src/size_list.c \
	src/token_utils.c \
	src/utils_alloc.c \
	src/utils_char.c \

OBJ_DIR = objs
OBJS = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))



LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) -pie -I include -L $(LIBFT_DIR) -o $(NAME) $(OBJS) -lft -lreadline

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
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
