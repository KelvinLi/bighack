objects = main.o
cflags = -Wall -Wextra -O2 -lpari
#-lm

bh : $(objects)
	$(CC) $(cflags) -o bh $(objects)

main.o : main.c
	$(CC) $(cflags) -c main.c
