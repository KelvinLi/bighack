/* Package for rectangular coordinates for use in turn_arr-to-path.c.


*** Key things to note:

-- The coordinates of any side in a coordinate system is uniquely
   determined in a way similar to how line integrals are defined in
   multivariable calculus - the line bordering any shape is defined to
   belong to the shape that is always to the left of the side. The
   direction of "left" is determined by the direction of motion.

-- One doesn't need to define the direction the ant is going in
   because given a certain starting direction, each side uniquely
   determines the direction the ant is moving (because the ant is
   always moving in counter-clockwise direction). This is true for
   every coordinate system.

-- This implementation in C also directly converts the paths computed
   into coordinates for the PNG output.


*/

#include "rect_coord.h"
#include <stdlib.h>
#include <string.h>

#undef NO_LINE_PAD

/* Sides */

int TOP = 0;
int RIGHT = 1;
int BOT = 2;
int LEFT = 3;


/* Functions that define motion for the ant */

void go_straight(int* ant_pos) {
    /* Updates the side the ant is on by going straight. */
    if ((ant_pos[2] & 3) == TOP) {
        ant_pos[0]--;
    } else if ((ant_pos[2] & 3) == RIGHT) {
        ant_pos[1]++;
    } else if ((ant_pos[2] & 3) == BOT) {
        ant_pos[0]++;
    } else if ((ant_pos[2] & 3) == LEFT) {
        ant_pos[1]--;
    }
}

void turn_left(int* ant_pos) {
    /* Updates the side the ant is on by turning left. */
    ant_pos[2] = (ant_pos[2]-1) & 3;
}

void compress_path_to_map(int* ant_pos, int** map, point_type *map_top_left, point_type *map_bot_right) {
    /* A function that takes a path and a map, calculates the pixels
       in the map to which to add counts, and adds the counts to the
       map iff the pixels are on the map. */

    int side = ant_pos[2];
    int x = (int) (ant_pos[0] + (map_bot_right->x + map_top_left->x)/2);
    int y = (int) (ant_pos[1] + (map_top_left->y + map_bot_right->y)/2);
    int map_width = map_bot_right->x - map_top_left->x;
    int map_height = map_top_left->y - map_bot_right->y;

    if (side == TOP && 0<=x && x<=map_width && 0<=y+1 && y+1<map_height) {
        map[y+1][x] += 1;
    } else if (side == RIGHT && 0<=x+1 && x+1<=map_width && 0<=y+1 && y+1<map_height) {
        map[y+1][x+1] += 1;
    } else if (side == BOT && 0<=x+1 && x+1<=map_width && 0<=y && y<map_height) {
        map[y][x+1] += 1;
    } else if (side == LEFT && 0<=x && x<=map_width && 0<=y && y<map_height) {
        map[y][x] += 1;
    }
}



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
        compress_path_to_map(ant_pos, map, map_top_left, map_bot_right);
        if (n == turn_arr[i]) {
            turn_left(ant_pos); i++;
        } else {
            go_straight(ant_pos);
        }
        n++;
    }

    // add the last turn (ie. always ends on a turn)
    if (ant_pos[0] >= 0 && ant_pos[0] < map_width && ant_pos[1] >= 0 && ant_pos[1] < map_height) {
        compress_path_to_map(ant_pos, map, map_top_left, map_bot_right);
    }        

    return map;
}
