#include "rect_coord.h"

int[][] make_map(int* turn_arr, int map_width, int map_height) {
    /* Takes a strictly-increasing set turn_arr (ie. the input from
       the prime number generator or other generator) and tests the
       natural numbers on them. If the number tested is in turn_arr,
       then tell the "ant" to turn left. Otherwise, continue going
       straight. Before each such calculation, a count for the current
       side the ant is on is added to a 2D array representing the
       number of times the point has been crossed. This is passed
       to the PNG renderer for the final mapping of an image. */

    int i = 0;
    int n = 1;
    int map[][];  // need to dynamically allocate
    int ant_pos[3] = {0,0,0};  // arbitrary starting side
    int next_turn;
    
    while (i < len(turn_arr)) {
        next_turn = turn_arr[i];
        compress_path_to_map(ant_pos, map);
        
        if (n == next_turn) {
            ant_pos = turn_left(ant_pos);
            ++i;
        } else {
            ant_pos = go_straight(ant_pos);
        }

        ++n;
    }

    // add the last turn (ie. always ends on a turn)
    compress_path_to_map(ant_pos, map);

    return map;
}

void compress_path_to_map(int* ant_pos, int[][] map) {
    /* A function that takes a path and a map, calculates the pixels
       in the map to which to add counts, and adds the counts to the
       map. */
    
}
