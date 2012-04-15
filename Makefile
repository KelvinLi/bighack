objects = main.o arith2.o
cflags = -Wall -Wextra -O2 -lpng -lpari
#-lm

bh : $(objects)
	$(CC) $(cflags) -o bh $(objects)

main.o : main.c
	$(CC) $(cflags) -c main.c

arith2.o : arith2.c
	$(CC) $(cflags) -c arith2.c
