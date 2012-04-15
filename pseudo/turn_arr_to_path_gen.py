# Comment these depending which to build
from rect_coord import *
#from tri_coord import *

def make_path(turn_arr):
    """Takes a strictly-increasing set turn_arr (ie. the input from
    the prime number generator or other generator) and tests the
    natural numbers on them. If the number tested is in turn_arr, then
    tell the "ant" to turn left. Otherwise, continue going straight.
    In either case, the path the "ant" takes is yielded as through the
    generator object this generator function creates. The generator
    yields tuples of the form
    
        coords: (x,y,side)

    """
    i,n = 0,1
    ant_pos = (0,0,0)  # arbitrary starting side
    while i < len(turn_arr):
        next_turn = turn_arr[i]
        yield ant_pos
        if n == next_turn:
            ant_pos = turn_left(ant_pos)
            i += 1
        else:
            ant_pos = go_straight(ant_pos)
        n += 1
    # add the last turn (ie. always ends on a turn)
    yield ant_pos
