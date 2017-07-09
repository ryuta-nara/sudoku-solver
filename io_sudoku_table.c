#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sudoku_parameters.h"
#include "sudoku_structures.h"

static FILE *GetFilePointer(char *file_name, char *mode) {
	FILE *p_file;

	if ( NULL == (p_file = fopen(file_name, mode)) ) {
		fprintf(stderr, "Error: %s: No such a file.\n", file_name);
		exit(EXIT_FAILURE);
	}

	return (p_file);
}

static int FGetCwithEOFCheck(FILE *p_file) {
	int ch;

	ch = fgetc(p_file);
	if ( EOF == ch ) {
		fprintf(stderr, "Error: Invalid Sudoku format.");
		fclose(p_file);
		exit(EXIT_FAILURE);
	}

	return ch;
}

static int AsciiToInteger( int ch ) {

	return ch - 0x30;
}

static unsigned int AsciiToDigitFlag( int ch ) {
	int integer;
	unsigned int digit_flag;

	integer = AsciiToInteger(ch);
	digit_flag = 1 << (integer - 1);

	return digit_flag;

}

static int DigitFlagToAscii( unsigned int digit_flag ) {
	int ascii;

	switch ( digit_flag ) {
		case 0x0:
			ascii = '0';
			break;
		case 0x1:
			ascii = '1';
			break;
		case 0x2:
			ascii = '2';
			break;
		case 0x4:
			ascii = '3';
			break;
		case 0x8:
			ascii = '4';
			break;
		case 0x10:
			ascii = '5';
			break;
		case 0x20:
			ascii = '6';
			break;
		case 0x40:
			ascii = '7';
			break;
		case 0x80:
			ascii = '8';
			break;
		case 0x100:
			ascii = '9';
			break;
		default:
			assert(0);
			break;
	}

	return ascii;
}

unsigned int PutDigitFromBinary(unsigned int data, unsigned int num_digits) {
	int i;
	unsigned int bit;
	unsigned int digit;

	if ( data == 0 ) {
		printf("0");
	} else {
		for (i = 0; i < num_digits; i++) {
			bit = data & (0x1 << i);
			if ( bit ) {
				digit = i + 1;
			} else {
				continue;
			}
			printf("%d", digit);
		}
	}

	return EXIT_SUCCESS;
}

static int PutData(unsigned int data[], unsigned int num_squares) {
	int i;

	for (i = 0; i < num_squares; i++) {
		printf("[%d] = ", i);
		PutDigitFromBinary(data[i], num_squares);
		if ( i < (num_squares - 1) ) {
			printf(", ");
		} else {
			printf("\n");
		}
	}

	return EXIT_SUCCESS;
}

static int PutRowData(unsigned int data[]) {
	PutData(data, MAX_ROW);

	return EXIT_SUCCESS;
}

static int PutColumnData(unsigned int data[]) {
	PutData(data, MAX_COLUMN);

	return EXIT_SUCCESS;
}

static int PutBoxData(unsigned int data[]) {
	PutData(data, MAX_BOX);

	return EXIT_SUCCESS;
}

static int PutSquaresData(unsigned int data[][MAX_COLUMN]) {
	int i;
	int j;

	for (i = 0; i < MAX_ROW; i++) {
		for (j = 0; j < MAX_COLUMN; j++) {
			printf("[%d][%d] = ", i, j);
			PutDigitFromBinary(data[i][j], MAX_ROW);
			if ( j < (MAX_COLUMN - 1) ) {
				printf(", ");
			} else {
				printf("\n");
			}
		}
	}

	return EXIT_SUCCESS;
}

static int PutCoordData(coordinate_t coord) {
	printf("[%d,%d]", coord.row, coord.column);

	return EXIT_SUCCESS;
}

static int PutFixedTemporaryDigit(unsigned int data) {
	PutDigitFromBinary(data, MAX_DIGIT);

	return EXIT_SUCCESS;
}

static int PutNumberBlankSquares(unsigned int num) {
	printf("num_blank_squares = %d\n", num);

	return EXIT_SUCCESS;
}

int PutGrid(grid_t grid) {

	printf("square_fixed_digits[ROW][COLUMN]\n");
	PutSquaresData(grid.square_fixed_digits);
	printf("square_invalid_digits[ROW][COLUMN]\n");
	PutSquaresData(grid.square_invalid_digits);
	printf("square_candidate_digits[ROW][COLUMN]\n");
	PutSquaresData(grid.square_candidate_digits);
	printf("row_fixed_digits[ROW]\n");
	PutColumnData(grid.row_fixed_digits);
	printf("row_candidate_digits[ROW]\n");
	PutColumnData(grid.row_candidate_digits);
	printf("column_fixed_digits[COLUMN]\n");
	PutRowData(grid.column_fixed_digits);
	printf("column_candidate_digits[COLUMN]\n");
	PutRowData(grid.column_candidate_digits);
	printf("box_fixed_digits[BOX]\n");
	PutBoxData(grid.box_fixed_digits);
	printf("box_candidate_digits[BOX]\n");
	PutBoxData(grid.box_candidate_digits);
	printf("is_no_candidate_square = %d\n", grid.is_no_candidate_square);
	PutNumberBlankSquares(grid.num_blank_squares);
	printf("square_fixed_temporary = ");
	PutCoordData(grid.square_fixed_temporary);
	printf("\nfixed_temporary_digit = ");
	PutFixedTemporaryDigit(grid.fixed_temporary_digit);
	printf("\n");

	return EXIT_SUCCESS;
}

static int InitSquaresData(unsigned int data[][MAX_COLUMN], unsigned int init) {
	int i;
	int j;

	for (i = 0; i < MAX_ROW; i++) {
		for (j = 0; j < MAX_COLUMN; j++) {
			data[i][j] = init;
		}
	}

	return EXIT_SUCCESS;
}

static int InitRowData(unsigned int data[], unsigned int init) {
	int i;

	for (i = 0; i < MAX_COLUMN; i++) {
		data[i] = init;
	}

	return EXIT_SUCCESS;
}

static int InitColumnData(unsigned int data[], unsigned int init) {
	int i;

	for (i = 0; i < MAX_ROW; i++) {
		data[i] = init;
	}

	return EXIT_SUCCESS;
}

static int InitBoxData(unsigned int data[], unsigned int init) {
	int i;

	for (i = 0; i < MAX_BOX; i++) {
		data[i] = init;
	}

	return EXIT_SUCCESS;
}

grid_t InitSquareFixedTemporary(grid_t grid) {
	grid.square_fixed_temporary.row = NONEXISTENCE;
	grid.square_fixed_temporary.column = NONEXISTENCE;
	grid.fixed_temporary_digit = 0;
	return grid;
}

grid_t InitGrid(grid_t grid) {
	InitSquaresData(grid.square_fixed_digits, 0);
	InitSquaresData(grid.square_invalid_digits, 0);
	InitSquaresData(grid.square_candidate_digits, 0);
	InitColumnData(grid.row_fixed_digits, 0);
	InitColumnData(grid.row_candidate_digits, 0);
	InitRowData(grid.column_fixed_digits, 0);
	InitRowData(grid.column_candidate_digits, 0);
	InitBoxData(grid.box_fixed_digits, 0);
	InitBoxData(grid.box_candidate_digits, 0);
	grid.is_no_candidate_square = 0;
	grid.num_blank_squares = MAX_ROW * MAX_COLUMN;
	grid = InitSquareFixedTemporary(grid);

	return grid;
}

static grid_t ReadSudokuFile(grid_t grid, char *file_name) {
	int ite_row;
	int ite_column;
	int ch;
	FILE *p_file;

	p_file = GetFilePointer(file_name, "r");

	for ( ite_row = 0; ite_row < MAX_ROW; ite_row++ ) {
		for ( ite_column = 0; ite_column < (MAX_COLUMN - 1); ite_column++ ) {
			ch = FGetCwithEOFCheck(p_file);
			if ( (ch >= '1' && ch <= '9') ) {
				grid.square_fixed_digits[ite_row][ite_column] = AsciiToDigitFlag(ch);
				grid.num_blank_squares--;
			} else if ( ch == ',' ) {
				grid.square_fixed_digits[ite_row][ite_column] = BLANK;
				ungetc(ch, p_file);
			} else {
				fprintf(stderr, "Error: Invalid charactor is found.\n");
				fprintf(stderr, "row = %d, column = %d. \n", ite_row, ite_column);
				fclose(p_file);
				exit(EXIT_FAILURE);
			}
			ch = FGetCwithEOFCheck(p_file);
			if ( ch != ',' ) {
				fprintf(stderr, "Error: ',' is not found.\n");
				fprintf(stderr, "row = %d, column = %d. \n", ite_row, ite_column);
				fclose(p_file);
				exit(EXIT_FAILURE);
			}
#ifdef DEBUG
			fprintf(stderr, "grid.square_fixed_digits[%d][%d] = %x\n", ite_row, ite_column, grid.square_fixed_digits[ite_row][ite_column]); 
#endif
		}
		/* ite_row == (MAX_ROW - 1) */
		ch = FGetCwithEOFCheck(p_file);
		if ( (ch >= '1' && ch <= '9') ) {
			grid.square_fixed_digits[ite_row][ite_column] = AsciiToDigitFlag(ch);
			grid.num_blank_squares--;
		} else if ( ch == '\n' ) {
			grid.square_fixed_digits[ite_row][ite_column] = BLANK;
			ungetc(ch, p_file);
		}
		ch = FGetCwithEOFCheck(p_file);
		if ( ch != '\n' ) {
			fprintf(stderr, "Error: Column number is invalid.\n");
			fprintf(stderr, "row = %d, column = %d. \n", ite_row, ite_column);
			fclose(p_file);
			exit(EXIT_FAILURE);
		}
#ifdef DEBUG
		fprintf(stderr, "grid.square_fixed_digits[%d][%d] = %x\n", ite_row, ite_column, grid.square_fixed_digits[ite_row][ite_column]); 
#endif
	}

	fclose(p_file);
	return grid;
}

int WriteSudokuFile(grid_t grid, char *file_name) {
	int ite_row;
	int ite_column;
	int ascii;
	FILE *p_file;

	p_file = GetFilePointer(file_name, "w");

	for ( ite_row = 0; ite_row < MAX_ROW; ite_row++ ) {
		for ( ite_column = 0; ite_column < MAX_COLUMN; ite_column++ ) {
			ascii = DigitFlagToAscii(grid.square_fixed_digits[ite_row][ite_column]);
			if ( ascii >= '1' && ascii <= '9' ) {
				fputc(ascii, p_file);
			}
			if ( ite_column < (MAX_COLUMN - 1) ) {
				fputc(',', p_file);
			} else {
				fputc('\n', p_file);
			}
		}
	}

	fclose(p_file);
	return EXIT_SUCCESS;
}

grid_t ReadSudokuProblem(grid_t grid) {

	grid = ReadSudokuFile(grid, SUDOKU_PROBLEM_FILE);

	return grid;
}

grid_t ReadSudokuAnswer(grid_t grid) {

	grid = ReadSudokuFile(grid, SUDOKU_ANSWER_FILE);

	return grid;
}

int WriteSudokuResult(grid_t grid) {

	WriteSudokuFile(grid, SUDOKU_RESULT_FILE);

	return EXIT_SUCCESS;
}

