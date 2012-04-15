# directions
POS_X = 0
POS_Y = 1
NEG_X = 2
NEG_Y = 3

def go_straight(ant_side, direction):
    """Updates the side the ant is on by going straight."""
    if direction == POS_X:
        ant_side[0] += 1
    elif direction == NEG_X:
        ant_side[0] -= 1
    elif direction == POS_Y:
        ant_side[1] += 1
    elif direction == NEG_Y:
        ant_side[1] -= 1
    else:
        raise Exception("Invalid direction supplied")
    return ant_side, direction

def turn_left(ant_side, direction):
    """Updates the side the ant is on by turning left and moving
    forward."""
    return go_straight(ant_side, (direction+1)%4)
