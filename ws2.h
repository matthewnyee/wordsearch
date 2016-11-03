char * build_board(char *filename, int *size);
void search(char *board, char input[30], int size);
void neighbor(int *k, int *l, char *board, int size, char input[30]);
void direction(int i, int j, int *k, int *l, char input[30], char *board, int size);
