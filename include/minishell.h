/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thchau <thchau@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:35:36 by thchau            #+#    #+#             */
/*   Updated: 2025/05/26 20:05:26 by thchau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include "../Libft/libft.h"
# include "../Libft/ft_printf/printf.h"
# include <sys/wait.h>
# include <stdbool.h>
#include <dirent.h>
#include <fnmatch.h>
# include <signal.h>
//# include "../test/mem_debug.h"

# define RST		"\033[0m" // Reset formatting
# define R			"\033[31m" // Red
#define BLUE		"\001\033[1;34m\002"
#define RESET		"\001\033[0m\002"

// ===============================
// ENUMS
// ===============================
// TOKENS - DUDE:

typedef enum e_cmd_type {
	CMD_NONE,
	CMD_AND_IF,
	CMD_OR_IF,
	CMD_PIPE
}   t_cmd_type;

typedef enum e_token
{
	CMD, //A command: ls, echo, cd
	WORD, //A generic word arguement
	PIPE, // |
	MORE, // >
	MOREMORE, // >>
	LESS, // <
	LESSLESS, // <<
	AND, // &&
	OR // ||
}   t_token;

typedef enum e_cmd_status
{
	CMD_EXIT = -1,
	CMD_SUCCESS = 0,
	CMD_FAILURE = 1
}   t_cmd_status;

// ===============================
// STRUCTS
// ===============================
typedef struct s_redir
{
	t_token			type;
	char			*filename;  // or delimiter for heredoc
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;	// arguments passed to execve or builtin
	t_redir			*redirs;	// list of redirections
	struct s_cmd	*next;	// next command in pipeline
	t_cmd_type		next_type;	// e.g. AND_IF, OR_IF, PIPE
}	t_cmd;


typedef struct s_pid_pipe_fd
{
	pid_t	pids[100];
	pid_t	pid;
	int		prev_fd;
	int		pipe_fd[2];
	int		child_count;
	
}	t_pid_pipe_fd;

// ===============================
// PARSER INTERFACE - András
// ===============================
t_cmd	*parse_input(char *input, int last_status, char **envp);
char	*complete_input(void);
void    print_cmds(t_cmd *cmd);
char    **ft_tokenize(char *input);
int     is_redirection(char *token);
int    handle_redirection(t_cmd *cmd, char **tokens, int *i);
int		validate_tokens(char **tokens);
void    free_split(char **arr);
char    **tokenize_line(const char *line);                            // optional internal
t_cmd   *build_cmd_list(char **tokens, char **envp);                 // optional internal
t_redir *parse_redirections(char **tokens, int *i);  
char	**handle_expansion_if_any(char *arg, int last_status, char **env); 
char	*expand_variables(const char *arg, int last_status, char **env);
char	*expand_one_var(char **p, int last_status, char **env);
void	setup_signals(void);
char	**expand_wildcard(const char *pattern);

// ===============================
// EXECUTOR INTERFACE - Thao
// ===============================
int		execute_commands(t_cmd *cmd_list, char ***envp, int *last_status);
int		is_builtin(const char *cmd);
int		execute_builtin(t_cmd *cmd, char ***envp, int *status);
int		process_pipe(t_cmd *cmd, char ***envp, int *last_status);
int		apply_redirections(t_redir *redirs);
int		process_heredoc(t_redir *redir);
int		cd_builtin(t_cmd *cmd);
int		pwd_builtin();
int		export_builtin(t_cmd *cmd, char ***envp);
int		echo_builtin(t_cmd *cmd, int *status);
int		unset_builtin(t_cmd *cmd, char ***envp);
int		env_builtin(char **envp);
int 	exit_builtin(t_cmd *cmd, char ***envp);
int		execute_single_command(t_cmd *cmd, char ***envp, int *last_status,
		bool should_fork);

// ===============================
// CLEANUP / UTILS
// ===============================
char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_strjoin_path(char *s1, char *s2);
void	free_split(char **arr);
void	free_cmd(t_cmd *cmd);
int		ft_strcmp(const char *s1, const char *s2);
char	**selection_sort(char **ar);
char	**clone_arr(char **ar);
void	log_error(const char *error, const char *function);
int		is_valid_identifier(const char *s);
void	print_sorted_env(char **env);
int		safe_dup2(int oldfd, int newfd, char *error, char *function);

#endif
