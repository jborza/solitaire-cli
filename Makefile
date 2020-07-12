CC=cc
sol:
	$(CC) -o solitaire -DUNICODE -Wall main.c -lncursesw -ltinfo
debug:
	$(CC) -g -o solitaire -DUNICODE -Wall main.c -lncursesw -ltinfo
ascii:
	$(CC) -o solitaire -Wall main.c -lncursesw -ltinfo

