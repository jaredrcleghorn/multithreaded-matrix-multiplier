#ifndef PACKAGE_DOT_PRODUCT_H
#define PACKAGE_DOT_PRODUCT_H

#include <pthread.h>

#include "../structs/Matrix.h"

// Package a dot product.
void* packageDotProduct(void* args);

typedef struct PackageDotProductArgument {
	int jobId;
	int innerDim;
	int rowNum;
	int colNum;
	Matrix** factors;
	int msgQueueId;
	pthread_mutex_t* numMsgsSentLock;
	int* numMsgsSent;
} PackageDotProductArgument;

// Create an argument with the given values.
PackageDotProductArgument* createPackageDotProductArgument(long jobId, int innerDim, int rowNum, int colNum, Matrix* factors[2], int msgQueueId, pthread_mutex_t* numMsgsSentLock, int* numMsgsSent);

// Free the memory that was allocated for the given argument.
void destroyPackageDotProductArgument(PackageDotProductArgument* arg);

#endif
