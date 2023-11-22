build:
	rm -rf dist
	mkdir dist
	cc src/main.c -o dist/main

scratchpad:
	rm -rf dist/scratchpad
	cc src/scratchpad.c -o dist/scratchpad
