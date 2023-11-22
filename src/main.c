#include <stdio.h>

#define RL_BUFSIZE 1024
char *_read_line(void)
{
	int bufsize = RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer) {
	 fprintf(stderr, "_: allocation error\n");
	 exit(EXIT_FAILURE);
	}

	while(1) {
		// read a character
		c = getchar();
		// if we hit EOF, replace it with a null characterand return
		if (c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		} else {
			buffer[position] = c;			
		}
		position++;

		// if we have exceeded the buffer, reallocate memory
		if (position >= bufsize) {
			bufsize += RL_BUFFSIZE;
			buffer = realloc(buffer, buffsize);
			if (!buffer) {
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
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
