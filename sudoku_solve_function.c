#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sudoku_parameters.h"
#include "sudoku_structures.h"
#include "io_sudoku_table.h"

static unsigned int GetFixedDigitsInRow(grid_t grid, unsigned int row) {
	int i;
	unsigned int fixed_digits = 0;

	for (i = 0; i < MAX_ROW; i++) {
		fixed_digits |= grid.square_fixed_digits[row][i];
	}

	return fixed_digits;
}

static unsigned int GetUnfixedDigitsInRow(grid_t grid, unsigned int row) {
	unsigned int fixed_digits;
	unsigned int unfixed_digits;

	fixed_digits = GetFixedDigitsInRow(grid, row);
	unfixed_digits = ~fixed_digits & MAX_BITS;

	return unfixed_digits;
}

static grid_t CalculateRowCandidates(grid_t grid, unsigned int row) {
	unsigned int unfixed_digits;

	unfixed_digits = GetUnfixedDigitsInRow(grid, row);
#ifdef DEBUG
	{
		printf("row %d: unfixed_digits = ", row);
		PutDigitFromBinary(unfixed_digits, MAX_DIGIT);
		printf("\n");
	}
#endif
	grid.row_candidate_digits[row] = unfixed_digits;

	return grid;
}

static unsigned int GetFixedDigitsInColumn(grid_t grid, unsigned int column) {
	int i;
	unsigned int fixed_digits = 0;

	for (i = 0; i < MAX_ROW; i++) {
		fixed_digits |= grid.square_fixed_digits[i][column];
	}

	return fixed_digits;
}

static unsigned int GetUnfixedDigitsInColumn(grid_t grid, unsigned int column) {
	unsigned int fixed_digits;
	unsigned int unfixed_digits;

	fixed_digits = GetFixedDigitsInColumn(grid, column);
	unfixed_digits = ~fixed_digits & MAX_BITS;

	return unfixed_digits;
}

static grid_t CalculateAllRowCandidates(grid_t grid) {
	int i;

	for ( i = 0; i < MAX_ROW; i++ ) {
		grid = CalculateRowCandidates(grid, i);
	}

	return grid;
}

static grid_t CalculateColumnCandidates(grid_t grid, unsigned int column) {
	unsigned int unfixed_digits;

	unfixed_digits = GetUnfixedDigitsInColumn(grid, column);
#ifdef DEBUG
	{
		printf("column %d: unfixed_digits = ", column);
		PutDigitFromBinary(unfixed_digits, MAX_DIGIT);
		printf("\n");
	}
#endif
	grid.column_candidate_digits[column] = unfixed_digits;

	return grid;
}

static grid_t CalculateAllColumnCandidates(grid_t grid) {
	int i;

	for ( i = 0; i < MAX_COLUMN; i++ ) {
		grid = CalculateColumnCandidates(grid, i);
	}

	return grid;
}

static coordinate_t GetUpperLeftCornerInBox(unsigned int box) {
	coordinate_t coord;

	switch (box) {
		case 0:
			coord.row = 0;
			coord.column = 0;
			break;
		case 1:
			coord.row = 0;
			coord.column = 3;
			break;
		case 2:
			coord.row = 0;
			coord.column = 6;
			break;
		case 3:
			coord.row = 3;
			coord.column = 0;
			break;
		case 4:
			coord.row = 3;
			coord.column = 3;
			break;
		case 5:
			coord.row = 3;
			coord.column = 6;
			break;
		case 6:
			coord.row = 6;
			coord.column = 0;
			break;
		case 7:
			coord.row = 6;
			coord.column = 3;
			break;
		case 8:
			coord.row = 6;
			coord.column = 6;
			break;
		default:
			assert(0);
			break;
	}

	return coord;
}

static unsigned int GetFixedDigitsInBox(grid_t grid, unsigned int box) {
	unsigned int r;
	unsigned int c;
	unsigned int fixed_digits = 0;
	coordinate_t coord;

	coord = GetUpperLeftCornerInBox(box);

	fixed_digits = 0;
	for (r = coord.row; r < (coord.row + BOX_ROW); r++) {
		for ( c = coord.column; c < (coord.column + BOX_COLUMN); c++) {
			fixed_digits |= grid.square_fixed_digits[r][c];
		}
	}

	return fixed_digits;
}

static unsigned int GetUnfixedDigitsInBox(grid_t grid, unsigned int box) {
	unsigned int fixed_digits = 0;
	unsigned int unfixed_digits;

	fixed_digits = GetFixedDigitsInBox(grid, box);
	unfixed_digits = ~fixed_digits & MAX_BITS;

	return unfixed_digits;
}

static grid_t CalculateBoxCandidates(grid_t grid, unsigned int box) {
	unsigned int unfixed_digits;

	unfixed_digits = GetUnfixedDigitsInBox(grid, box);
#ifdef DEBUG
	{
		printf("box %d: unfixed_digits = ", box);
		PutDigitFromBinary(unfixed_digits, MAX_DIGIT);
		printf("\n");
	}
#endif
	grid.box_candidate_digits[box] = unfixed_digits;

	return grid;
}

static grid_t CalculateAllBoxCandidates(grid_t grid) {
	int i;

	for ( i = 0; i < MAX_BOX; i++ ) {
		grid = CalculateBoxCandidates(grid, i);
	}

	return grid;
}

static unsigned int GetBoxPlace(unsigned int row, unsigned int column) {
	unsigned int box;

	if ( row >= 0 && row < BOX_ROW ) {
		if ( column >= 0 && column < BOX_COLUMN ) {
			box = 0;
		} else if ( column >= BOX_COLUMN && column <= (2 * BOX_COLUMN - 1 ) ) {
			box = 1;
		} else if ( column >= (2 * BOX_COLUMN) && column <= (3 * BOX_COLUMN - 1) ) {
			box = 2;
		} else {
			assert(0);
		}
	} else if ( row >= BOX_ROW && row <= (2 * BOX_ROW - 1) ) {
		if ( column >= 0 && column < 3 ) {
			box = 3;
		} else if ( column >= BOX_COLUMN && column <= (2 * BOX_COLUMN - 1 ) ) {
			box = 4;
		} else if ( column >= (2 * BOX_COLUMN) && column <= (3 * BOX_COLUMN - 1) ) {
			box = 5;
		} else {
			assert(0);
		}
	} else if ( row >= (2 * BOX_ROW) && row <= (3 * BOX_ROW - 1 ) ) {
		if ( column >= 0 && column < 3 ) {
			box = 6;
		} else if ( column >= BOX_COLUMN && column <= (2 * BOX_COLUMN - 1 ) ) {
			box = 7;
		} else if ( column >= (2 * BOX_COLUMN) && column <= (3 * BOX_COLUMN - 1) ) {
			box = 8;
		} else {
			assert(0);
		}
	} else {
		assert(0);
	}

	return box;
}

static unsigned int GetCommonCandidates(grid_t grid, unsigned int row, unsigned int column) {
	int box;
	unsigned int common_candidates;

	box = GetBoxPlace(row, column);
	common_candidates = grid.row_candidate_digits[row] & grid.column_candidate_digits[column] & grid.box_candidate_digits[box];

	return common_candidates;
}

static grid_t CalculateSquareCandidates(grid_t grid, unsigned int row, unsigned column) {
	unsigned int common_candidates;

	if ( 0 != grid.square_fixed_digits[row][column] ) {
		grid.square_candidate_digits[row][column] = 0;
	} else {
		common_candidates = GetCommonCandidates(grid, row, column);
		grid.square_candidate_digits[row][column] = common_candidates & ~grid.square_invalid_digits[row][column];
		if ( 0 == grid.is_no_candidate_square ) {
			if ( 0 == grid.square_candidate_digits[row][column] ) {
				grid.is_no_candidate_square = 1;
			} else {
				grid.is_no_candidate_square = 0;
			}
		}
	}

	return grid;
}

static grid_t CalculateAllSquareCandidates(grid_t grid) {
	unsigned int r;
	unsigned int c;

	for ( r = 0; r < MAX_ROW; r++) {
		for ( c = 0; c < MAX_COLUMN; c++) {
			grid = CalculateSquareCandidates(grid, r, c);
			if ( 1 == grid.is_no_candidate_square ) {
				goto OUT;
			}
		}
	}

OUT:
	return grid;
}

static grid_t CalculateCandidates(grid_t grid) {
	grid = CalculateAllRowCandidates(grid);
	grid = CalculateAllColumnCandidates(grid);
	grid = CalculateAllBoxCandidates(grid);
	grid = CalculateAllSquareCandidates(grid);
	return grid;
}

static unsigned int GetUniqueRowCandidate(grid_t grid, unsigned int row, unsigned int column) {
	unsigned int c;
	unsigned int unique_candidate;
		
	unique_candidate = grid.square_candidate_digits[row][column];
#ifdef DEBUG
	{
		int j;
		printf("base [%d,%d]\n", row, column);
		PutDigitFromBinary(unique_candidate, 9);
		printf("\n");
		for ( j = 0; j < MAX_COLUMN; j++ ) {
			printf("[%d,%d]unique_candidate = ", row, j);
			PutDigitFromBinary(grid.square_candidate_digits[row][j], 9);
			printf("\n");
		}
	}
#endif
	for ( c = 0; c < MAX_COLUMN; c++ ) {
		if ( 0 == grid.square_fixed_digits[row][column] ) {
			if ( c != column ) {
				unique_candidate &= ~grid.square_candidate_digits[row][c];
			}
		}
	}
	return unique_candidate;
}

static unsigned int GetUniqueColumnCandidate(grid_t grid, unsigned int row, unsigned int column) {
	unsigned int r;
	unsigned int unique_candidate;
		
	unique_candidate = grid.square_candidate_digits[row][column];
	for ( r = 0; r < MAX_ROW; r++ ) {
		if ( 0 == grid.square_fixed_digits[row][column] ) {
			if ( r != row ) {
				unique_candidate &= ~grid.square_candidate_digits[r][column];
			}
		}
	}
	return unique_candidate;
}

static unsigned int GetUniqueBoxCandidate(grid_t grid, unsigned int row, unsigned int column) {
	unsigned int r;
	unsigned int c;
	unsigned int box;
	unsigned int unique_candidate;
	coordinate_t coord;
		
	unique_candidate = grid.square_candidate_digits[row][column];
	box = GetBoxPlace(row, column);
	coord = GetUpperLeftCornerInBox(box);
	for (r = coord.row; r < (coord.row + BOX_ROW); r++) {
		for ( c = coord.column; c < (coord.column + BOX_COLUMN); c++) {
			if ( 0 == grid.square_fixed_digits[row][column] ) {
				if ( (r != row) || (c != column) ) {
						unique_candidate &= ~grid.square_candidate_digits[r][c];
				}
			}
		}
	}
	return unique_candidate;
}

typedef enum {
	NONE,
	UNIQUE,
	MULTIPLE
} STATE_DIGIT;

static STATE_DIGIT CheckUniqueDigit(unsigned int unique_candidate) {
	int i;
	unsigned int digit;
	unsigned int flag = 0;
	STATE_DIGIT state;

	for (i = 0; i < MAX_DIGIT; i++) {
		digit = 1 << i;
#ifdef DEBUG
		printf("CheckUniqueDigit unique_candidate & digit = %x\n", unique_candidate & digit);
#endif
		if ( 0 != (unique_candidate & digit) ) {
			flag++;
		}
	}
	if ( flag == 0 ) {
		state = NONE;
	} else if ( flag == 1 ) {
		state = UNIQUE;
	} else {
		state = MULTIPLE;
	}
#ifdef DEBUG
	printf("CheckUniqueDigit state = %x\n", state);
#endif

	return state;
}

static grid_t PutUniqueDigit(grid_t grid, unsigned int row, unsigned int column,  unsigned int unique_candidate) {
	unsigned int unique_digit;
	STATE_DIGIT state;

	state = CheckUniqueDigit(unique_candidate);
	if ( state == UNIQUE ) {
		unique_digit = unique_candidate;
		grid.square_fixed_digits[row][column] = unique_digit;
		grid.num_blank_squares--;
	}
	return grid;
}

static grid_t FixSquareDigit(grid_t grid, unsigned int row, unsigned int column) {
	unsigned int unique_candidate;
	unsigned int prev_num_blank_squares;

	prev_num_blank_squares = grid.num_blank_squares;
	grid = PutUniqueDigit(grid, row, column, grid.square_candidate_digits[row][column]);
	if ( 0 != prev_num_blank_squares - grid.num_blank_squares ) {
		goto OUT;
	}

	unique_candidate = GetUniqueRowCandidate(grid, row, column);
#ifdef DEBUG
	printf("GetUniqueRowCandidate(grid, %d, %d)\n", row, column);
	PutDigitFromBinary(unique_candidate, 9);
	printf("\n");
#endif
	grid = PutUniqueDigit(grid, row, column, unique_candidate);
	if ( 0 != prev_num_blank_squares - grid.num_blank_squares ) {
		goto OUT;
	}
	unique_candidate = GetUniqueColumnCandidate(grid, row, column);
#ifdef DEBUG
	printf("GetUniqueColumnCandidate(grid, %d, %d)\n", row, column);
	PutDigitFromBinary(unique_candidate, 9);
	printf("\n");
#endif
	grid = PutUniqueDigit(grid, row, column, unique_candidate);
	if ( 0 != prev_num_blank_squares - grid.num_blank_squares ) {
		goto OUT;
	}
	unique_candidate = GetUniqueBoxCandidate(grid, row, column);
#ifdef DEBUG
	printf("GetUniqueBoxCandidate(grid, %d, %d)\n", row, column);
	PutDigitFromBinary(unique_candidate, 9);
	printf("\n");
#endif
	grid = PutUniqueDigit(grid, row, column, unique_candidate);
	if ( 0 != prev_num_blank_squares - grid.num_blank_squares ) {
		goto OUT;
	}

OUT:
	return grid;
}

static grid_t FixAllSquareDigit(grid_t grid) {
	unsigned int r;
	unsigned int c;

	for (r = 0; r < MAX_ROW; r++) {
		for (c = 0; c < MAX_COLUMN; c++) {
			if ( BLANK == grid.square_fixed_digits[r][c] ) {
				grid = FixSquareDigit(grid, r, c);
			}
		}
	}

	return grid;
}

static grid_t FillSquares(grid_t grid) {
	unsigned int prev_num_blank_squares = 0;

	while ( 0 != (grid.num_blank_squares - prev_num_blank_squares) ) {
		grid = CalculateCandidates(grid);
#ifdef DEBUG
		fprintf(stderr, "CalculateCandidates(grid)\n");
		PutGrid(grid);
#endif
		if ( 1 == grid.is_no_candidate_square ) {
			break;
		}
		prev_num_blank_squares = grid.num_blank_squares;
		grid = FixAllSquareDigit(grid);
#ifdef DEBUG
		fprintf(stderr, "FixAllSquareDigit(grid)\n");
		PutGrid(grid);
#endif
	}

	return grid;
}

static coordinate_t GetBlankSquare(grid_t grid) {
	unsigned int r;
	unsigned int c;
	coordinate_t coord;

	for ( r = 0; r < MAX_ROW; r++ ) {
		for ( c = 0; c < MAX_COLUMN; c++ ) {
			if ( 0 == grid.square_fixed_digits[r][c] ) {
				goto OUT;
			}
		}
	}
OUT:
	if ( r != MAX_ROW || c != MAX_ROW ) {
		coord.row = r;
		coord.column = c;
	} else {
		coord.row = NONEXISTENCE;
		coord.column = NONEXISTENCE;
	}
#ifdef DEBUG
		fprintf(stderr, "GetBlankSquare(grid)\n");
		fprintf(stderr, "BlankSquare is [%d,%d]\n", coord.row, coord.column);
#endif
	return coord;
}

static unsigned int GetMinimumSquareCandidate(unsigned int square_candidate) {
	unsigned int i;
	unsigned int digit;

	for ( i = 0; i < MAX_DIGIT; i++ ) {
		digit = 1 << i;
		if ( 0 != (square_candidate & digit) ) {
			break;
		}
	}
#ifdef DEBUG
	printf("GetMinimumSquareCandidate = %x\n", digit);
//	PutDigitFromBinary(digit, MAX_DIGIT);
//	printf("\n");
#endif
	return digit;
}

static grid_t PutTemporaryFixedDigit(grid_t grid, coordinate_t coord, unsigned int digit) {
	grid.square_fixed_digits[coord.row][coord.column] = digit;
	grid.square_fixed_temporary = coord;
	grid.fixed_temporary_digit = digit;
	grid.num_blank_squares--;
	return grid;
}

static grid_t PutInvalidDigit(grid_t grid, coordinate_t coord, unsigned int digit) {
	grid.square_invalid_digits[coord.row][coord.column] |= digit;
	return grid;
}

static grid_t FixTemporaryDigit(grid_t grid) {
	coordinate_t coord;
	unsigned int tmp_digit;

	coord = GetBlankSquare(grid);
	if ( (coord.row != NONEXISTENCE) && (coord.column != NONEXISTENCE) ) {
		tmp_digit = GetMinimumSquareCandidate(grid.square_candidate_digits[coord.row][coord.column]);
		assert(MULTIPLE != CheckUniqueDigit(tmp_digit));
		grid = PutTemporaryFixedDigit(grid, coord, tmp_digit);
		grid = PutInvalidDigit(grid, coord, tmp_digit);
	}
#ifdef DEBUG
		fprintf(stderr, "FixTemporaryDigit(grid)\n");
		PutGrid(grid);
#endif
	return grid;
}

static grid_t EraseTemporaryDigit(grid_t grid) {
	grid.square_fixed_digits[grid.square_fixed_temporary.row][grid.square_fixed_temporary.column] = 0;
	InitSquareFixedTemporary(grid);
	grid.num_blank_squares++;
	return grid;
}

static grid_t SolveSudoku(grid_t grid) {
	grid_t prev_grid;

	grid = FillSquares(grid);
	if ( 1 == grid.is_no_candidate_square ) {
		printf("Sudoku problem is invalid.\n");
		return grid;
	}
#ifdef DEBUG
	fprintf(stderr, "FillSquares(grid)\n");
	fprintf(stderr, "num_blank_squares = %d\n", grid.num_blank_squares);
#endif
	while ( 0 != grid.num_blank_squares ) {
		grid = FixTemporaryDigit(grid);
		prev_grid = grid;
		grid = FillSquares(grid);
		if ( 1 == grid.is_no_candidate_square ) {
			grid = prev_grid;
			grid = EraseTemporaryDigit(grid);
			grid.is_no_candidate_square = 0;
			grid = CalculateCandidates(grid);
		}
	}

	return grid;
}

int solve_sudoku(int argc, char *argv[]) {

	grid_t grid;

	grid = InitGrid(grid);
#ifdef DEBUG
	fprintf(stderr, "InitGrid\n");
#endif
	PutGrid(grid);
	grid = ReadSudokuProblem(grid);
#ifdef DEBUG
	fprintf(stderr, "ReadSudokuProblem\n");
#endif
	PutGrid(grid);
	grid = SolveSudoku(grid);
#ifdef DEBUG
	fprintf(stderr, "SolveSudoku(grid)\n");
#endif
	PutGrid(grid);
	WriteSudokuResult(grid);

	return EXIT_SUCCESS;
}

