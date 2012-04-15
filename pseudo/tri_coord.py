"""Package for triangular coordinates for use in turn_arr-to-path.py.


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


*** Details for the triangular coordinate system:

-- The triangular system is defined by the upside triangular balanced
   on its tip like so:
   ----
   \  /
    \/

-- The positive-x direction is defined as moving from left to right
   along the path of the top side of the triangle, while the
   positive-y direction is defined as going from SE to NW by following
   the left side of the triangle. As such, moving along the right side
   of the triangle results in increasing both the x AND y positions of
   the ant.

"""

## Sides ##

TOP = 0
RIGHT = 1
LEFT = 2


## Functions that define motion for the ant ##

def go_straight(ant_pos):
    """Updates the side the ant is on by going straight.
    """
    assert 0 <= ant_pos[2] <= 2, "Invalid side supplied"
    if ant_pos[2] == TOP:
        return ant_pos[0]-1,ant_pos[1],ant_pos[2]
    elif ant_pos[2] == RIGHT:
        return ant_pos[0]+1,ant_pos[1]+1,ant_pos[2]
    elif ant_pos[2] == LEFT:
        return ant_pos[0],ant_pos[1]-1,ant_pos[2]

def turn_left(ant_pos):
    """Updates the side the ant is on by turning left.
    """
    assert 0 <= ant_pos[2] <= 2, "Invalid side supplied"
    return ant_pos[0],ant_pos[1],(ant_pos[2]-1)%3
