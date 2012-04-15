#include "rect_coord.h"

int** make_map(int* turn_arr, int map_width, int map_height) {
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
    int ant_pos[3] = {0,0,0};  // arbitrary starting side
    int next_turn;
    
    /* Allocate memory for map */
    int** map = malloc(map_height*sizeof(int*));
    if (map == NULL) {
        return NULL;  // malloc fail
    }
    for (i = 0; i < map_height; i++) {
        map[i] = malloc(map_width*sizeof(int));
        if (map[i] == NULL) {
            free2darray(map,i);
            return NULL;  // malloc fail
        }
    }

    while (i < length(turn_arr)) {
        next_turn = turn_arr[i];
        compress_path_to_map(ant_pos, map);
        
        if (n == next_turn) {
            turn_left(ant_pos);
            ++i;
        } else {
            go_straight(ant_pos);
        }

        ++n;
    }

    // add the last turn (ie. always ends on a turn)
    compress_path_to_map(ant_pos, map);

    return map;
}

