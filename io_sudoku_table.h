#ifndef _SUDOKU_IO_SUDOKU_TABLE
#define _SUDOKU_IO_SUDOKU_TABLE

#ifdef DEBUG
unsigned int PutDigitFromBinary(unsigned int data, unsigned int num_digits);
#endif
int PutGrid(grid_t grid);
grid_t InitSquareFixedTemporary(grid_t grid);
grid_t InitGrid(grid_t grid);
int WriteSudokuFile(grid_t grid, char *file_name);
grid_t ReadSudokuProblem(grid_t grid);
grid_t ReadSudokuAnswer(grid_t grid);
int WriteSudokuResult(grid_t grid);

#endif /* _SUDOKU_IO_SUDOKU_TABLE */

