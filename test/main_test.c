#include "../include/minishell.h"
//#include "mem_debug.h"
#include <string.h>
#include <ctype.h>

t_cmd *parse_input(char *line)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;

    cmd->argv = malloc(sizeof(char *) * 64); // arbitrary capacity for now
    if (!cmd->argv)
        return NULL;

    cmd->redirs = NULL;

    int i = 0;
    int in_single_quote = 0, in_double_quote = 0;
    char *start = NULL;
    t_redir *last_redir = NULL;

    for (char *p = line; *p;)
    {
        // Skip whitespace
        while (*p == ' ' || *p == '\t' || *p == '\n')
            *p++ = '\0';

        if (*p == '\0') break;

        // Handle quotes
        if (*p == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            start = ++p;
            while (*p && (*p != '\'' || in_single_quote == 0))
                p++;
            in_single_quote = 0;
            *p++ = '\0';
            cmd->argv[i++] = strdup(start);
        }
        else if (*p == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            start = ++p;
            while (*p && (*p != '"' || in_double_quote == 0))
                p++;
            in_double_quote = 0;
            *p++ = '\0';
            cmd->argv[i++] = strdup(start);
        }
        else if (*p == '>' || *p == '<')
        {
            t_redir *new_redir = malloc(sizeof(t_redir));
            if (!new_redir)
                return NULL;

            new_redir->next = NULL;

            if (*p == '>' && *(p + 1) == '>')
            {
                new_redir->type = MOREMORE;
                p += 2;
            }
            else if (*p == '<' && *(p + 1) == '<')
            {
                new_redir->type = LESSLESS;
                p += 2;
            }
            else if (*p == '>')
            {
                new_redir->type = MORE;
                p++;
            }
            else
            {
                new_redir->type = LESS;
                p++;
            }

            // Skip whitespace
            while (*p == ' ' || *p == '\t')
                p++;

            // Capture filename
            start = p;
            while (*p && *p != ' ' && *p != '\t' && *p != '\n' && *p != '<' && *p != '>')
                p++;
            new_redir->filename = strndup(start, p - start);

            // Append to redir list
            if (!cmd->redirs)
                cmd->redirs = new_redir;
            else
                last_redir->next = new_redir;

            last_redir = new_redir;
        }
        else
        {
            start = p;
            while (*p && *p != ' ' && *p != '\t' && *p != '\n' && *p != '<' && *p != '>')
                p++;
            if (*p)
                *p++ = '\0';
            cmd->argv[i++] = strdup(start);
        }
    }

    cmd->argv[i] = NULL;
    cmd->next = NULL;
    cmd->next_type = CMD_NONE;

    return cmd;
}


char *join_path(const char *dir, const char *cmd)
{
	char *full = malloc(strlen(dir) + strlen(cmd) + 2);
	if (!full) return NULL;
	sprintf(full, "%s/%s", dir, cmd);
	return full;
}

char *resolve_command(const char *cmd)
{
	if (strchr(cmd, '/')) // already a path
		return strdup(cmd);

	char *path = getenv("PATH");
	if (!path)
		return NULL;

	char *path_copy = strdup(path);
	char *token = strtok(path_copy, ":");

	while (token)
	{
		char *full_path = join_path(token, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return full_path;
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return NULL;
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	char	*line;
	t_cmd	*cmd;
	int		last_exit_status = 0;
	int		stdout_bk;

	// Duplicate environment
	char **shell_envp = clone_arr(envp);
	if (!shell_envp)
		return (1);

	while (1)
	{
		line = readline("my_shell> ");
		if (!line)
			break;
		if (*line)
			add_history(line);

		cmd = parse_input(line);
		free(line);

		if (!cmd || !cmd->argv || !cmd->argv[0])
		{
			free_cmd(cmd);
			continue;
		}
		// Save the original STDOUT_FILENO
		stdout_bk = dup(STDOUT_FILENO);
		if (stdout_bk == -1)
		{
			log_error("Error saving STDOUT_FILENO", "dup");
			return (CMD_FAILURE);
		}
		if (apply_redirections(cmd->redirs) == CMD_FAILURE)
		{
			last_exit_status = 1;
			free_cmd(cmd);
			continue;
		}
		execute_commands(cmd, shell_envp, &last_exit_status);
		// Restore the original STDOUT_FILENO
		if (dup2(stdout_bk, STDOUT_FILENO) == -1)
		{
			log_error("Error restoring STDOUT_FILENO", "dup2");
			free_cmd(cmd);
			continue;
		}
		close(stdout_bk);
		/*if (is_builtin(cmd->argv[0]))
		{
			last_exit_status = execute_builtin(cmd, &shell_envp, last_exit_status);
		}
		else
		{
			printf("I'M HERE.......SURPRISED......\n");
			pid_t pid = fork();
			if (pid == 0)
			{
				char *path = resolve_command(cmd->argv[0]);
				if (!path || execve(path, cmd->argv, shell_envp) == -1)
				{
					perror("execve");
					exit(EXIT_FAILURE);
				}
			}
			else
				waitpid(pid, &status, 0);
		}*/
		free_cmd(cmd);
		cmd = NULL;
	}
	free_split(shell_envp);
//	print_allocs(); // to see if anything is leaking
	return 0;
}