#include "turn_arr_to_path.h"
#include "rect_coord.h"
#include <stdlib.h>
#include <string.h>

void free2darray(int **arr, int height) {
    int i;
    for (i = 0; i < height; i++) {
        free(arr[i]);
    }
    free(arr);
}

int** make_map(int* turn_arr, int turn_arr_size, point_type *map_top_left, point_type *map_bot_right) {
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
    int map_width = map_bot_right->x - map_top_left->x + 1;
    int map_height = map_top_left->y - map_bot_right->y + 1;

    /* Starting position not arbitrary - relative to map grid selection.
       The origin is defined at the bottom left corner. */
    int ant_pos[3] = {0, 0, 3};
    
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
        memset(map[i], 0, map_width*sizeof(int));
    }

    i = 0;
    while (i < turn_arr_size) {
        compress_path_to_map(ant_pos, map, map_width, map_height);
        if (n == turn_arr[i]) {
            turn_left(ant_pos); i++;
        } else {
            go_straight(ant_pos);
        }
        n++;
    }

    // add the last turn (ie. always ends on a turn)
    if (ant_pos[0] >= 0 && ant_pos[0] < map_width && ant_pos[1] >= 0 && ant_pos[1] < map_height) {
        compress_path_to_map(ant_pos, map, map_width, map_height);
    }        

    return map;
}
