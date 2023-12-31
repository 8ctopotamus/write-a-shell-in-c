#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

/* Function declarations for builtin shell commands */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

/* list of builtin commands, followed by their corresponding functions */
char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};
 
int (*builtin_func[]) (char **) = {
	&lsh_cd,
	&lsh_help,
	&lsh_exit
};

int lsh_num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

/* builtin function implementations */
int lsh_cd(char **args)
{
	if (args[1] == NULL) {
		fprintf(stderr, "lsh: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("lsh");
		}
	}
	return 1;
}

int lsh_help(char **args)
{
	int i;
	printf("My LSH\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");
	for (i = 0; i < lsh_num_builtins(); i++) {
		printf("  %s\n", builtin_str[i]);
	}
	printf("Use the man command for infomation on other programs");
	return 1;
}

int lsh_exit(char **args)
{
	return 0;
}

#define RL_BUFSIZE 1024
char *_read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0; // have getline allocate a buffer for us

	if (getline(&line, &bufsize, stdin) == -1) {
		if (feof(stdin)) {
			exit(EXIT_SUCCESS); // we recieved an EOF
		} else {
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}

	return line;
}

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
char **_split_line(char *line)
{
	int bufsize = TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;
		
		if (position >= bufsize) {
			bufsize += TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "allocation err\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

int _execute(char **args) 
{
	int i;
	if (args[0] == NULL) {
		// an empty command was entered
		return 1;
	}
	for (i = 0; i < lsh_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}
	return lsh_launch(args);
}

int _launch(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// child process
		if (execvp(args[0], args) == -1) {
			perror("child process error");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// error forking
		perror("error forking");
	} else {
		// parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

void _loop(void) {
	char *line;
	char **args;
	int status;
	
	do {
		printf("> ");
		line = _read_line();
		args = _split_line(line);
		status = _execute(args);

		free(line);
		free(args);
	} while (status);
}

int main(int argc, char **argv) {
	// load config files, if any

	// run command loop
	_loop();

	// perform any shutdown/cleanup
	
  return EXIT_SUCCESS;
}
