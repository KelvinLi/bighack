from tkinter import *
root = Tk()

input = [(125 ,125 ,1), (125 ,126 ,1), (125 ,126 ,0), (125, 126, 3), (125, 126, 2), (125, 126, 1)]

def increase_hex(hex1):
    result = "#" + hex(int(hex1[1:], 16) + int("100000", 16))[2:]
    if int(result[1:], 16) <= 16711680:
        return result

def move_on(coords):
    """Inputs generator of coordinates and creates lines on canvas. 
    Does not overwrite objects, changes the color to more red instead"""
    frame.focus_set()
    for coord in coords:
        X, Y, side = coord
        X, Y = X*2, Y*2
        if side == 0:
            x1, x2, y1, y2 = X+1, X-1, Y+1, Y+1
        if side == 1:
            x1, x2, y1, y2 = X+1, X+1, Y-1, Y+1
        if side == 2:
            x1, x2, y1, y2 = X-1, X+1, Y-1, Y-1
        if side == 3:
            x1, x2, y1, y2 = X-1, X-1, Y+1, Y-1
        if x1 == x2 and y1 == y2:
            raise ValueError("Illegal move")
        occupied = DrawingBoard.find_overlapping((x1+x2)/2,(y1+y2)/2,(x1+x2)/2,(y1+y2)/2)
        if occupied:
            if len(occupied)!= 1:
                raise TypeError("Object bad positioned")
            new_hex = increase_hex(DrawingBoard.gettags(occupied[0])[0])
            DrawingBoard.itemconfig(occupied[0],fill = new_hex, tag = new_hex)
            DrawingBoard.tag_raise(occupied[0])
            
        else:
            DrawingBoard.create_line(x1, y1, x2, y2, fill = "#000000", tag = "#000000")


    
            

    
Bdraw = Button(root, text= "Draw", command = lambda: move_on(input), anchor = CENTER)
Bdraw.pack()

DrawingBoard = Canvas(root, height = 500, width = 500, bg = "#006100", scrollregion = (400, 250, 0, 0), confine = True)
DrawingBoard.pack()

frame = Frame(root, width = 700, height = 700)
frame.pack()


root.mainloop()
