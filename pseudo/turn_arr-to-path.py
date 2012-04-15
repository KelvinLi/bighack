# Comment these depending which to build
from rect_coord import *
#from tri_coord import *

def make_path(turn_arr):
    """Takes a strictly-increasing set turn_arr (ie. the input from
    the prime number generator or other generator) and tests the
    natural numbers on them. If the number tested is in turn_arr, then
    tell the "ant" to turn left. Otherwise, continue going straight.
    In either case, the path the "ant" takes is added to a hash table.
    If the path the ant takes is already in the hash table (ie. the
    "ant" takes a path that it has taken before), then add to the
    count of the entry in the hash table. Finally, the function
    returns a hash table of the path the ant took consisting of the
    form:
    
        hash_table: {(x,y,side): count, ...}

    """
    i = 0
    hash_table = {}
    ant_side = (0,0,0)  # arbitrary starting side
    direction = 0  # arbitrary starting direction
    n = 1  # consider c optimize
    for i in range(len(turn_arr)):  # consider c optimize
        next_turn = turn_arr[i]
        while n != next_turn:
            hash_table[ant_side] = hash_table.get(ant_side,0) + 1
            ant_side, direction = go_straight(ant_side, direction)
            n += 1
        ant_side, direction = turn_left(ant_side, direction)
        n += 1
        i += 1
    return hash_table
