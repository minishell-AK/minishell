/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:00:00 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/20 04:34:06 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_HEREDOC,
	TOK_END,
	TOK_ERROR
}	t_token_type;

typedef enum e_seg_type
{
	SEG_UNQUOTED,
	SEG_SINGLE_QUOTED,
	SEG_DOUBLE_QUOTED
}	t_seg_type;

typedef struct s_seg
{
	t_seg_type		type;
	char			*str;
	struct s_seg	*next;
}	t_seg;

typedef struct s_token
{
	t_token_type	type;
	char			*raw;
	int				no_expand;
	int				in_double;
	t_seg			*segs;
	struct s_token	*next;
}	t_token;

typedef struct s_exp
{
	char			**envp;
	int				last_status;
	char			*out;
	size_t			out_len;
	size_t			cap;
}	t_exp;

typedef struct s_word_ctx
{
	const char		*line;
	size_t			len;
	t_seg			**segs;
	t_seg			**last;
}	t_word_ctx;

t_token	*lexer_tokenize(const char *line);
void	token_free_all(t_token *tok);
t_token	*token_new(t_token_type type, const char *raw);
void	token_append(t_token **head, t_token *node);
int		append_word_token(const char *line, size_t start, size_t end,
			t_token **head);
int		collect_word(const char *line, size_t *i, size_t len,
			t_token **head);
int		handle_redir(const char *line, size_t *i, size_t len,
			t_token **head);
void	expand_tokens(t_token *head, char **envp, int last_status);
char	*get_env_value(char **envp, const char *name);
char	*int_to_str(int n);
int		set_env_var(char ***envp_ref, const char *name, const char *value);
int		expand_buf(t_exp *ctx, const char *str);
int		expand_char(t_exp *ctx);
int		expand_status(t_exp *ctx, size_t *i);
void	rebuild_raw(t_token *tk);
char	*expand_line(const char *s, char **envp, int last_status);

typedef struct s_seg_ctx
{
	t_seg			**segs;
	t_seg			**last;
	t_seg_type		type;
	const char		*src;
	size_t			len;
}	t_seg_ctx;

int		add_seg_to_list(t_seg_ctx *ctx);
int		add_seg_with_ctx(t_seg_ctx *ctx);
int		collect_quoted(t_word_ctx *ctx, size_t *i, char quote);
int		collect_unquoted(t_word_ctx *ctx, size_t *i);
int		process_quoted_word(t_word_ctx *ctx, size_t *i);
int		finalize_token(t_seg *segs, t_token **head);
void	free_seg_list(t_seg *segs);

#endif
