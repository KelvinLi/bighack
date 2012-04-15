"""Package for rectangular coordinates for use in turn_arr-to-path.py.


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


"""

## Sides ##

TOP = 0
RIGHT = 1
BOT = 2
LEFT = 3


## Functions that define motion for the ant ##

def go_straight(ant_pos):
    """Updates the side the ant is on by going straight.
    """
    assert 0 <= ant_pos[2] <= 3, "Invalid side supplied"
    if ant_pos[2] == TOP:
        return ant_pos[0]-1,ant_pos[1],ant_pos[2]
    elif ant_pos[2] == RIGHT:
        return ant_pos[0],ant_pos[1]+1,ant_pos[2]
    elif ant_pos[2] == BOT:
        return ant_pos[0]+1,ant_pos[1],ant_pos[2]
    elif ant_pos[2] == LEFT:
        return ant_pos[0],ant_pos[1]-1,ant_pos[2]

def turn_left(ant_pos):
    """Updates the side the ant is on by turning left.
    """
    assert 0 <= ant_pos[2] <= 3, "Invalid side supplied"
    return ant_pos[0],ant_pos[1],(ant_pos[2]-1)%4
