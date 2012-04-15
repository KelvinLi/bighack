objects = main.o rect_coord.o turn_arr_to_path.o
cflags = -Wall -Wextra -O0 -g -lpng -lpari
#-lm

bh : $(objects)
	$(CC) $(cflags) -o bh $(objects)

main.o : main.c
	$(CC) $(cflags) -c main.c

rect_coord.o : rect_coord.c
	$(CC) $(cflags) -c rect_coord.c

turn_arr_to_path.o : turn_arr_to_path.c
	$(CC) $(cflags) -c turn_arr_to_path.c
