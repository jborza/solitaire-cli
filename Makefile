CC=cc
sol:
	$(CC) -o solitaire -Wall *.c -lncursesw -ltinfo
debug:
	$(CC) -g -o solitaire -Wall *.c -lncursesw -ltinfo

