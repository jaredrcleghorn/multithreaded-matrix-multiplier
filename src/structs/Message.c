#include <stdlib.h>

#include "Message.h"

Message* createMessage(long type, int jobId, int innerDim, int rowNum, int colNum, int numDataElements) {
	Message* msg = malloc(sizeof(Message) + sizeof(int) * numDataElements);

	msg->type = type;
	msg->jobId = jobId;
	msg->innerDim = innerDim;
	msg->rowNum = rowNum;
	msg->colNum = colNum;

	return msg;
}

void destroyMessage(Message* msg) {
	free(msg);
	return;
}
