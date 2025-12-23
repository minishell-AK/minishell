/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshi <kyoshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:26:53 by kakubo-l          #+#    #+#             */
/*   Updated: 2025/12/20 13:18:58 by kyoshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include <sys/types.h>
# include <stddef.h>

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_hdoc_ctx
{
	const char	*delimiter;
	int			expand;
	char		**envp;
	char		*template;
}	t_hdoc_ctx;

char	*create_heredoc(t_token *delim_token, char **envp);
void	free_commands(t_cmd *cmds);

t_cmd	*parse_tokens(t_token *tokens, char ***envp);
t_cmd	*cmd_new(void);
t_cmd	*ensure_cmd(t_cmd **head, t_cmd **cur);
t_token	*parse_word_token(t_token *tk, t_cmd **head, t_cmd **cur, char ***envp);
t_token	*parse_redir_token(t_token *tk, t_cmd **head, t_cmd **cur, char **envp);
t_token	*parse_pipe_token(t_token *tk, t_cmd **head, t_cmd **cur);
t_token	*parse_error_token(t_token *tk, t_cmd **head);

/* prototypes used by heredoc helpers (defined across parser_heredoc files) */
int		add_arg(t_cmd *cmd, const char *arg);
int		add_redir(t_cmd *cmd, t_redir_type type, const char *target);
int		write_heredoc_entry(int fd, char *line, int expand, char **envp);
int		build_tmp_name(char *name, pid_t pid, int seq);
int		try_create_tmp(const char *name, char *out, size_t out_sz);
int		heredoc_read_loop(int fd, t_hdoc_ctx *ctx);
int		open_unique_tmpfile(char *out, size_t out_sz);
int		process_heredoc_line_ctx(int fd, char *line, t_hdoc_ctx *ctx);

#endif
