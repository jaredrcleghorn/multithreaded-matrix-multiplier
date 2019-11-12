#ifndef MATRIX_H
#define MATRIX_H

typedef struct Matrix {
	int numRows;
	int numCols;
	int** array;
} Matrix;

// Create a matrix with the given numbers of rows and columns.
Matrix* createMatrix(int numRows, int numCols);

// Create the matrix described by the given file.
void* createMatrixFromFile(void* file);

// Print the given matrix to the given file.
void printMatrixToFile(Matrix* matrix, FILE* file);

// Free the memory that was allocated for the given matrix.
void destroyMatrix(Matrix* matrix);

#endif
