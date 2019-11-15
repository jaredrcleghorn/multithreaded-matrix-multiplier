#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>

#include "computeDotProduct.h"
#include "../structs/Message.h"

#define MAX_NUM_DATA_ELEMENTS 100
#define MAX_MSG_BODY_SIZE sizeof(Message) + sizeof(int) * MAX_NUM_DATA_ELEMENTS - sizeof(long)

void* computeDotProduct(void* _arg) {
	ComputeDotProductArgument* arg = (ComputeDotProductArgument*)_arg;

	// Create the message.
	Message* msg = createMessage(0, 0, 0, 0, 0, MAX_NUM_DATA_ELEMENTS);

	while (true) {
		// Receive the message.
		if(msgrcv(arg->msgQueueId, msg, MAX_MSG_BODY_SIZE, 1, 0) == -1) {
			fprintf(stderr, "message recieving error: %s\n", strerror(errno));
			continue;
		}

		// Calculate the size of the message's body.
		int msgBodySize = sizeof(Message) + sizeof(int) * 2 * msg->innerDim - sizeof(long);

		// Log the message as received.
		printf("Receiving job id %d type 1 size %d\n", msg->jobId, msgBodySize);

		// Increment the number of messages received.
		pthread_mutex_lock(arg->numMsgsReceivedLock);
		++(*arg->numMsgsReceived);
		pthread_mutex_unlock(arg->numMsgsReceivedLock);

		// Change the message's type.
		msg->type = 2;

		int dotProduct = 0;

		// Compute the dot product.
		for (int i = 0; i < msg->innerDim; ++i) {
			dotProduct += msg->data[i] * msg->data[msg->innerDim + i];
		}

		// Log the dot product.
		printf("Sum for cell %d,%d is %d\n", msg->rowNum, msg->colNum, dotProduct);

		// Add the dot product to the message.
		msg->data[0] = dotProduct;

		// Recalculate the size of the message's body.
		msgBodySize = sizeof(Message) + sizeof(int) - sizeof(long);

		// Send the message.
		int returnCode = msgsnd(arg->msgQueueId, msg, msgBodySize, 0);

		// Optionally, log the message as sent.
		if (!arg->suppressSentMsg) {
			printf("Sent job ID %d, type 2, size %d (rc = %d).\n", msg->jobId, msgBodySize, returnCode);
		}

		// Increment the number of messages sent.
		pthread_mutex_lock(arg->numMsgsSentLock);
		++(*arg->numMsgsSent);
		pthread_mutex_unlock(arg->numMsgsSentLock);
	}

	return NULL;
}
