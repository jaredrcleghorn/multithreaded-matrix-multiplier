#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>

#include "processDotProduct.h"
#include "../structs/Message.h"

#define MSG_BODY_SIZE sizeof(Message) + sizeof(int) - sizeof(long)

void* processDotProduct(void* _arg) {
	ProcessDotProductArgument* arg = (ProcessDotProductArgument*)_arg;

	// Create the message.
	Message* msg = createMessage(0, 0, 0, 0, 0, 1);

	// Recieve the message.
	if (msgrcv(arg->msgQueueId, msg, MSG_BODY_SIZE, 2, 0) == -1) {
		fprintf(stderr, "message recieving error: %s\n", strerror(errno));
		return NULL;
	}

	// Log the message as recieved.
	printf("Recieving job id %d type 2 size %lu\n", msg->jobId, MSG_BODY_SIZE);

	// Increment the number of messages received.
	pthread_mutex_lock(arg->numMsgsReceivedLock);
	++(*arg->numMsgsReceived);
	pthread_mutex_unlock(arg->numMsgsReceivedLock);

	// Add the dot product to the product matrix.
	arg->product->array[msg->rowNum][msg->colNum] = msg->data[0];

	return NULL;
}
