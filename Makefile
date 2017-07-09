# Define macro
CC	= gcc -Wall -g
#CC	= gcc -Wall -g -DDEBUG
OBJS	= main.o io_sudoku_table.o sudoku_solve_function.o
#OBJS_BMP    = function_bmp.o
HDS	= sudoku_parameters.h sudoku_structures.h io_sudoku_table.h sudoku_solve_function.h
#HDS_BMP	= function_bmp.h

# Generate rule
main.out: main.o $(OBJS)
	$(CC) -o $@ main.o io_sudoku_table.o sudoku_solve_function.o

clean: 
	rm -fr *.o *.out *.exe *.stackdump

main.o	: $(HDS)
io_sudoku_table.o	: $(HDS)
sudoku_solve_function.o	: $(HDS)
#function_bmp.o  : $(HDS_BMP)

