objects = main.o rect_coord.o
cflags = -Wall -Wextra -O2 -lpng -lpari
#-lm

bh : $(objects)
	$(CC) $(cflags) -o bh $(objects)

main.o : main.c
	$(CC) $(cflags) -c main.c

rect_coord.o : rect_coord.c
	$(CC) $(cflags) -c rect_coord.c
