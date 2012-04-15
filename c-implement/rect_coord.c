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


/* Sides */

int TOP = 0;
int RIGHT = 1;
int BOT = 2;
int LEFT = 3;


/* Functions that define motion for the ant */

void go_straight(int* ant_pos) {
    /* Updates the side the ant is on by going straight. */
    if (ant_pos[2] == TOP) {
        ant_pos[0]--;
    } else if (ant_pos[2] == RIGHT) {
        ant_pos[1]++;
    } else if (ant_pos[2] == BOT) {
        ant_pos[0]++;
    } else if (ant_pos[2] == LEFT) {
        ant_pos[1]--;
    }
}

void turn_left(int* ant_pos) {
    /* Updates the side the ant is on by turning left. */
    ant_pos[2] = (ant_pos[2]-1)%4;
}

void compress_path_to_map(int* ant_pos, int** map) {
    /* A function that takes a path and a map, calculates the pixels
       in the map to which to add counts, and adds the counts to the
       map iff the pixels are on the map. */
    int side = ant_pos[2];
    if (side == TOP) {
        
    }
}
