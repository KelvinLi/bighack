struct point { int x; int y; };
typedef struct point point_type;

enum cell_side { TOP, RIGHT, BOT, LEFT };

struct ant { int x; int y; enum cell_side side; };

void go_straight(struct ant* ant_pos);
void turn_left(struct ant* ant_pos);
void compress_path_to_map(struct ant* ant_pos, int** map, point_type *map_top_left, point_type *map_bot_right);


void free2darray(int **arr, int height);
int** make_map(int* turn_arr, int turn_arr_size, point_type *map_top_left, point_type *map_bot_right);
