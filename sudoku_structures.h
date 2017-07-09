#ifndef _SUDOKU_STRUCTURES
#define _SUDOKU_STRUCTURES

#include "sudoku_parameters.h"

typedef struct {
	unsigned int row;
	unsigned int column;
} coordinate_t;

typedef struct {
	unsigned int square_fixed_digits[MAX_ROW][MAX_COLUMN];
	unsigned int square_invalid_digits[MAX_ROW][MAX_COLUMN];
	unsigned int square_candidate_digits[MAX_ROW][MAX_COLUMN];
	unsigned int row_fixed_digits[MAX_ROW];
	unsigned int row_candidate_digits[MAX_ROW];
	unsigned int column_fixed_digits[MAX_COLUMN];
	unsigned int column_candidate_digits[MAX_COLUMN];
	unsigned int box_fixed_digits[MAX_BOX];
	unsigned int box_candidate_digits[MAX_BOX];
	unsigned int is_no_candidate_square;
	unsigned int num_blank_squares;
	coordinate_t square_fixed_temporary;
	unsigned int fixed_temporary_digit;
} grid_t;

#endif /* _SUDOKU_STRUCTURES */

