#ifndef COMPUTE_DOT_PRODUCT_H
#define COMPUTE_DOT_PRODUCT_H

#include <stdbool.h>

void* computeDotProduct(void* _suppressSentMsg);

typedef struct ComputeDotProductArgument {
	int msgQueueId;
	bool suppressSentMsg;
	pthread_mutex_t* numMsgsReceivedLock;
	int* numMsgsReceived;
	pthread_mutex_t* numMsgsSentLock;
	int* numMsgsSent;
} ComputeDotProductArgument;

#endif
