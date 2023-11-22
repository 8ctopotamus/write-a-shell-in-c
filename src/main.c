#include <stdio.h>

#define RL_BUFSIZE 1024
char *_read_line(void)
{
	int bufsize = RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer) {
		
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
