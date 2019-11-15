#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct Message {
	long type;
	int jobId;
	int innerDim;
	int rowNum;
	int colNum;
	int data[];
} Message;

// Create a message with the given values and number of data elements.
Message* createMessage(long type, int jobId, int innerDim, int rowNum, int colNum, int numDataElements);

// Free the memory that was allocated for the given message.
void destroyMessage(Message* msg);

#endif
