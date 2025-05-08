#include "../include/minishell.h"
#include "mem_debug.h"
#include <string.h>

static int	env_cmp(const void *a, const void *b)
{
	const char *s1 = *(const char **)a;
	const char *s2 = *(const char **)b;
	return strcmp(s1, s2);
}

void	print_sorted_env(char **envp)
{
	int		count = 0;
	char	**sorted_env;
	char	*eq;

	// Count entries
	while (envp[count])
		count++;

	// Duplicate envp to sort
	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return ;

	for (int i = 0; i < count; i++)
		sorted_env[i] = strdup(envp[i]);
	sorted_env[count] = NULL;

	// Sort
	qsort(sorted_env, count, sizeof(char *), env_cmp);

	// Print in format: export VAR="value"
	for (int i = 0; i < count; i++)
	{
		eq = strchr(sorted_env[i], '=');
		if (eq)
		{
			*eq = '\0';
			printf("export %s=\"%s\"\n", sorted_env[i], eq + 1);
			*eq = '='; // Restore
		}
		else
			printf("export %s\n", sorted_env[i]);
		free(sorted_env[i]);
	}
	free(sorted_env);
}

t_cmd *parse_input(char *line)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd) return NULL;

	int count = 0;
	char *tmp = strdup(line);
	char *token = strtok(tmp, " \t\n");
	while (token)
	{
		count++;
		token = strtok(NULL, " \t\n");
	}
	free(tmp);

	cmd->argv = malloc(sizeof(char *) * (count + 1));
	if (!cmd->argv) return NULL;

	int i = 0;
	token = strtok(line, " \t\n");
	while (token)
	{
		cmd->argv[i++] = strdup(token);
		token = strtok(NULL, " \t\n");
	}
	cmd->argv[i] = NULL;
	cmd->redirs = NULL;
	return cmd;
}

char **dup_envp(char **envp)
{
	int i = 0;
	char **copy;

	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return NULL;

	for (int j = 0; j < i; j++) {
		copy[j] = strdup(envp[j]);
		if (!copy[j]) {
			// free everything before returning NULL
			for (int k = 0; k < j; k++)
				free(copy[k]);
			free(copy);
			return NULL;
		}
	}
	copy[i] = NULL;
	return copy;
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

	// Duplicate environment
	char **shell_envp = dup_envp(envp);
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
			execute_builtin(cmd, &shell_envp);
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