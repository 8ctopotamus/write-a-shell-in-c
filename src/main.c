#include <stdio.h>
#include <string.h>

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
