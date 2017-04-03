CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c99 -c $(DEBUG)
LFLAGS = -Wall -std=c99 $(DEBUG)

sim03 : main.o configReader.o programReader.o helperFunctions.o pcbUtils.o
	$(CC) $(LFLAGS) main.o configReader.o programReader.o helperFunctions.o pcbUtils.o -lpthread -o sim03

main.o : main.c configReader.h programReader.h helperFunctions.h structures.h pcbUtils.h
	$(CC) $(CFLAGS) main.c

configReader.o : configReader.c helperFunctions.h structures.h
	$(CC) $(CFLAGS) configReader.c

programReader.o : programReader.c helperFunctions.h structures.h
	$(CC) $(CFLAGS) programReader.c

helperFunctions.o : helperFunctions.c
	$(CC) $(CFLAGS) helperFunctions.c

pcbUtils.o : pcbUtils.c structures.h
	$(CC) $(CFLAGS) pcbUtils.c

clean:
	\rm *.o sim03