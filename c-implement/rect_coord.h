struct point { int x, y; };
struct rect_region { struct point top_left, bot_right; };
enum cell_side { TOP, RIGHT, BOT, LEFT };
struct ant { int x, y; enum cell_side side; };

int rect_region_width(struct rect_region *region);
int rect_region_height(struct rect_region *region);

void go_straight(struct ant* ant_pos);
void turn_left(struct ant* ant_pos);
void compress_path_to_map(struct ant* ant_pos, int** map, struct rect_region *map_region);


void free2darray(int **arr, int height);
int** make_map(int* turn_arr, int turn_arr_size, struct rect_region *map_region);
