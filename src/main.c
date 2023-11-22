#include <stdio.h>

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
