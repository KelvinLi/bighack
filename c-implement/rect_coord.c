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

int rect_region_width(struct rect_region *region) {
    return region->bot_right.x - region->top_left.x + 1;
}

int rect_region_height(struct rect_region *region) {
    return region->top_left.y - region->bot_right.y + 1;
}

/* Functions that define motion for the ant */
void go_straight(struct ant* ant_pos) {
    /* Updates the side the ant is on by going straight. */
    switch(ant_pos->side & 3) {
        case TOP:   ant_pos->x --; break;
        case RIGHT: ant_pos->y ++; break;
        case BOT:   ant_pos->x ++; break;
        case LEFT:  ant_pos->y --; break;
        default:    return; /* this is an error case */
    }
}

void turn_left(struct ant* ant_pos) {
    /* Updates the side the ant is on by turning left. */
    ant_pos->side = (ant_pos->side - 1) & 3;
}

void compress_path_to_map(struct ant *ant_pos, int **map, struct rect_region *map_region) {
    /* A function that takes a path and a map, calculates the pixels
       in the map to which to add counts, and adds the counts to the
       map iff the pixels are on the map. */

    int x = ant_pos->x - map_region->top_left.x;
    int y = -ant_pos->y + map_region->top_left.y;
    int map_width = rect_region_width(map_region);
    int map_height = rect_region_height(map_region);

    switch(ant_pos->side) {
        case TOP:        break;
        case RIGHT: x++; break;
        case BOT:   x++; y++; break;
        case LEFT:  y++; break;
        default:    return; /* error case */
    }
    if (0<=x && x<=map_width && 0<=y && y<map_height)
        map[y][x] ++;
}

void free2darray(int **arr, int height) {
    int i;
    for (i = 0; i < height; i++) {
        free(arr[i]);
    }
    free(arr);
}

int **make_map(int *turn_arr, int turn_arr_size, struct rect_region *map_region) {
    /* Takes a strictly-increasing set turn_arr (ie. the input from
       the prime number generator or other generator) and tests the
       natural numbers on them. If the number tested is in turn_arr,
       then tell the "ant" to turn left. Otherwise, continue going
       straight. Before each such calculation, a count for the current
       side the ant is on is added to a 2D array representing the
       number of times the point has been crossed. This is passed
       to the PNG renderer for the final mapping of an image. */

    int i;
    int n;
    const int map_width = rect_region_width(map_region);
    const int map_height = rect_region_height(map_region);

    /* Starting position not arbitrary - relative to map grid selection.
       The origin is defined at the bottom left corner. */
    struct ant ant_pos = {.x = 0, .y = 0, .side = RIGHT};
    
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

    i = 0; n = 1;
    while (i < turn_arr_size) {
        compress_path_to_map(&ant_pos, map, map_region);
        if (n == turn_arr[i]) {
            turn_left(&ant_pos); i++;
        } else {
            go_straight(&ant_pos);
        }
        n++;
    }

    // add the last turn (ie. always ends on a turn)
    if (0<=ant_pos.x && ant_pos.x<map_width && 0<=ant_pos.y && ant_pos.y<map_height) {
        compress_path_to_map(&ant_pos, map, map_region);
    }        

    return map;
}
