#ifndef _SUDOKU_PARAMETERS
#define _SUDOKU_PARAMETERS

#define MAX_COLUMN 9
#define MAX_ROW 9
#define MAX_BOX 9
#define MAX_SQUARES (MAX_ROW * MAX_COLUMN)
#define MAX_DIGIT 9
#define BOX_ROW 3
#define BOX_COLUMN 3
#define MAX_BITS 0x1FF
#define NONEXISTENCE ~MAX_BITS

#define BLANK 0

#define SUDOKU_PROBLEM_FILE "./sample/sudoku_problem.txt"
#define SUDOKU_ANSWER_FILE "./sample/sudoku_answer.txt"
#define SUDOKU_RESULT_FILE "./sample/sudoku_result.txt"

#endif /* _SUDOKU_PARAMETERS */

