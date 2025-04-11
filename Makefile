all:
	gcc -I src/include main.c -L src/lib -lSDL3 -o pong.exe