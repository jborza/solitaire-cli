CC=cc
sol:
	$(CC) -o solitaire -Wall main.c -lncursesw -ltinfo
debug:
	$(CC) -g -o solitaire -Wall main.c -lncursesw -ltinfo

