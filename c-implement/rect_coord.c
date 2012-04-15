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

void compress_path_to_map(int* ant_pos, int** map, int map_width, int map_height) {
    /* A function that takes a path and a map, calculates the pixels
       in the map to which to add counts, and adds the counts to the
       map iff the pixels are on the map. */

    int side = ant_pos[2];
    int x = ant_pos[0] + map_width/2;
    int y = ant_pos[1] + map_height/2;

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
