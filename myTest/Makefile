# http://web.cse.ohio-state.edu/~reeves.92/CSE2421au12/SlidesDay22.pdf
CC=gcc

all: passR

passR: passR.o loadLinkList.o
	$(CC) -g -o passR loadLinkList.o passR.o -DUNICODE -Wall -lncursesw -ltinfo
loadLinkList.o : loadLinkList.c
	$(CC)    -c loadLinkList.c
passR.o : passR.c
	$(CC)    -c passR.c
clean: 
	rm -rf passR *.o 
run:
	./passR
