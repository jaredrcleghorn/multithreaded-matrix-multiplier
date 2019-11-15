#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#include "packageDotProduct.h"
#include "../structs/Message.h"

void* packageDotProduct(void* _arg) {
	PackageDotProductArgument* arg = (PackageDotProductArgument*)_arg;

	// Calculate the number of data elements for the message.
	int numDataElements = 2 * arg->innerDim;

	// Create the message.
	Message* msg = createMessage(1, arg->jobId, arg->innerDim, arg->rowNum, arg->colNum, numDataElements);

	// Add the data to the message.
	for (int i = 0; i < msg->innerDim; ++i) {
		msg->data[i] = arg->factors[0]->array[msg->rowNum][i];
		msg->data[msg->innerDim + i] = arg->factors[1]->array[i][msg->colNum];
	}

	// Calculate the size of the message's body.
	int msgBodySize = sizeof(Message) + sizeof(int) * numDataElements - sizeof(long);

	// Send the message.
	int returnCode = msgsnd(arg->msgQueueId, msg, msgBodySize, 0);

	// Log the message as sent.
	printf("Sending job id %d type 1 size %d (rc=%d)\n", msg->jobId, msgBodySize, returnCode);

	// Increment the number of messages sent.
	pthread_mutex_lock(arg->numMsgsSentLock);
	++(*arg->numMsgsSent);
	pthread_mutex_unlock(arg->numMsgsSentLock);

	// Destroy the message.
	destroyMessage(msg);

	return NULL;
}

PackageDotProductArgument* createPackageDotProductArgument(long jobId, int innerDim, int rowNum, int colNum, Matrix* factors[2], int msgQueueId, pthread_mutex_t* numMsgsSentLock, int* numMsgsSent) {
	PackageDotProductArgument* arg = malloc(sizeof(PackageDotProductArgument));

	arg->jobId = jobId;
	arg->innerDim = innerDim;
	arg->rowNum = rowNum;
	arg->colNum = colNum;
	arg->factors = factors;
	arg->msgQueueId = msgQueueId;
	arg->numMsgsSentLock = numMsgsSentLock;
	arg->numMsgsSent = numMsgsSent;

	return arg;
}

void destroyPackageDotProductArgument(PackageDotProductArgument* arg) {
	free(arg);
	return;
}
