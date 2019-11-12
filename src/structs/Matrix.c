#include <stdio.h>
#include <stdlib.h>

#include "Matrix.h"

Matrix* createMatrix(int numRows, int numCols) {
	Matrix* matrix = malloc(sizeof(Matrix));

	matrix->numRows = numRows;
	matrix->numCols = numCols;
	matrix->array = malloc(sizeof(int*) * numRows);

	for (int i = 0; i < numRows; ++i) {
		matrix->array[i] = malloc(sizeof(int) * numCols);
	}

	return matrix;
}

void* createMatrixFromFile(void* _file) {
	FILE* file = (FILE*)_file;

	int numRows;
	int numCols;

	fscanf(file, "%d %d\n", &numRows, &numCols);

	Matrix* matrix = createMatrix(numRows, numCols);

	for (int i = 0; i < numRows; ++i) {
		for (int j = 0; j < numCols; ++j) {
			fscanf(file, " %d", &matrix->array[i][j]);
		}
	}

	return matrix;
}

void printMatrixToFile(Matrix* matrix, FILE* file) {
	int lastRowIndex = matrix->numRows - 1;
	int lastColIndex = matrix->numCols - 1;

	if (matrix->numRows > 0) {
		for (int i = 0; i < lastRowIndex; ++i) {
			for (int j = 0; j < matrix->numCols; ++j) {
				fprintf(file, "%d ", matrix->array[i][j]);
			}
		}

		for (int i = 0; i < lastColIndex; ++i) {
			fprintf(file, "%d ", matrix->array[lastRowIndex][i]);
		}

		fprintf(file, "%d", matrix->array[lastRowIndex][lastColIndex]);
	}

	return;
}

void destroyMatrix(Matrix* matrix) {
	for (int i = 0; i < matrix->numRows; ++i) {
		free(matrix->array[i]);
	}

	free(matrix->array);
	free(matrix);

	return;
}
