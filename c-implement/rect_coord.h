struct point { int x; int y; };
typedef struct point point_type;
void go_straight(int* ant_pos);
void turn_left(int* ant_pos);
void compress_path_to_map(int* ant_pos, int** map, point_type *map_top_left, point_type *map_bot_right);


void free2darray(int **arr, int height);
int** make_map(int* turn_arr, int turn_arr_size, point_type *map_top_left, point_type *map_bot_right);
