struct point { int x; int y; };
typedef struct point point_type;
void free2darray(int **arr, int height);
int** make_map(int* turn_arr, int turn_arr_size, point_type *map_top_left, point_type *map_bot_right);
