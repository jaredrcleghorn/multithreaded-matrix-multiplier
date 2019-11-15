#ifndef PROCESS_DOT_PRODUCT_H
#define PROCESS_DOT_PRODUCT_H

#include "../structs/Matrix.h"

void* processDotProduct(void* arg);

typedef struct ProcessDotProductArgument {
	int msgQueueId;
	Matrix* product;
	pthread_mutex_t* numMsgsReceivedLock;
	int* numMsgsReceived;
} ProcessDotProductArgument;

#endif
