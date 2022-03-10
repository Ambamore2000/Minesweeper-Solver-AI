

void solve(struct box **board, int *input_row, int *input_col, char *input_type, int rows, int cols) {
    int hidden_and_flag_count = 0;
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            if (board[x][y].is_hidden || board[x][y].is_flagged) {
                hidden_and_flag_count++;
            }
        }
    }
    return (hidden_and_flag_count == mines_count);
}