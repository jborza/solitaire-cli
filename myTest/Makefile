CC=cc
passR: passR.c
	$(CC) -g -o passR -DUNICODE -Wall passR.c -lncursesw -ltinfo
nodebug: passR.c
	$(CC)    -o passR -DUNICODE -Wall passR.c -lncursesw -ltinfo
nodascii: passR.c
	$(CC)    -o passR           -Wall passR.c -lncursesw -ltinfo
ascii: passR.c
	$(CC) -g -o passR           -Wall passR.c -lncursesw -ltinfo
clean: 
	rm -f passR 
