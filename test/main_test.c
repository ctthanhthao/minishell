#include "../include/minishell.h"
//#include "mem_debug.h"
#include <string.h>
#include <ctype.h>

t_cmd *parse_input(char *line)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd) return NULL;

    int count = 0;
    char *tmp = strdup(line);
    int in_single_quote = 0, in_double_quote = 0;

    // First pass: count tokens
    for (char *p = tmp; *p; p++)
    {
        if (*p == '\\') // Skip the next character
        {
            p++;
            continue;
        }
        if (*p == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (*p == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        else if (!in_single_quote && !in_double_quote && (*p == ' ' || *p == '\t' || *p == '\n'))
        {
            *p = '\0';
            if (p > tmp && *(p - 1) != '\0')
                count++;
        }
    }
    if (tmp[0] != '\0')
        count++;
    free(tmp);

    cmd->argv = malloc(sizeof(char *) * (count + 1));
    if (!cmd->argv) return NULL;

    // Second pass: extract tokens and handle variable expansion
    int i = 0;
    in_single_quote = 0;
    in_double_quote = 0;
    char *start = NULL;

    for (char *p = line; *p; p++)
    {
        if (*p == '\\') // Handle escape sequences
        {
            if (!start)
                start = p;
            memmove(p, p + 1, strlen(p)); // Remove the backslash
            continue;
        }
        if (*p == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            if (!in_single_quote && start)
            {
                cmd->argv[i++] = strndup(start, p - start);
                start = NULL;
            }
            else if (in_single_quote)
                start = p + 1;
        }
        else if (*p == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            if (!in_double_quote && start)
            {
                cmd->argv[i++] = strndup(start, p - start);
                start = NULL;
            }
            else if (in_double_quote)
                start = p + 1;
        }
        else if (!in_single_quote && in_double_quote && *p == '$') // Handle variable expansion
        {
            char *var_start = p + 1;
            while (*p && (isalnum(*p) || *p == '_'))
                p++;
            char *var_name = strndup(var_start, p - var_start);
            char *var_value = getenv(var_name);
            free(var_name);
            if (var_value)
            {
                size_t len = strlen(var_value);
                memmove(var_start - 1 + len, p, strlen(p) + 1); // Shift the rest of the string
                memcpy(var_start - 1, var_value, len);          // Insert the variable value
                p = var_start - 1 + len - 1;                   // Adjust pointer
            }
            p--; // Adjust for the loop increment
        }
        else if (!in_single_quote && !in_double_quote && (*p == ' ' || *p == '\t' || *p == '\n'))
        {
            *p = '\0';
            if (start)
            {
                cmd->argv[i++] = strdup(start);
                start = NULL;
            }
        }
        else if (!start)
            start = p;
    }
    if (start)
        cmd->argv[i++] = strdup(start);

    cmd->argv[i] = NULL;
    cmd->redirs = NULL;
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
	int		status = 0;
	int		last_exit_status = 0;

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

		if (is_builtin(cmd->argv[0]))
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
		}
		free_cmd(cmd);
		cmd = NULL;
	}

	free_envp(shell_envp);
//	print_allocs(); // to see if anything is leaking
	return 0;
}