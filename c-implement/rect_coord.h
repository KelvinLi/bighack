struct point { int x, y; };
struct rect_region { struct point top_left, bot_right; };
enum cell_side { TOP, RIGHT, BOT, LEFT };
struct ant { struct point pos; enum cell_side side; };

int rect_region_width(struct rect_region *region);
int rect_region_height(struct rect_region *region);

int** make_map(int* turn_arr, int turn_arr_size, struct rect_region *map_region);
