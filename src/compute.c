#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>

#include "constants.h"
#include "functions/computeDotProduct.h"

#define USAGE_ERR_MSG "usage: compute thread_pool_size [-n]"

// Use global variables so that the information is available to the signal handler.
int numMsgsReceived = 0;
int numMsgsSent = 0;

void handleSigint(int signal);
void printUsageErrorMessage();

int main(int argc, char* argv[]) {
	signal(SIGINT, handleSigint);

	if (argc < 2) {
		fprintf(stderr, "%s\n", USAGE_ERR_MSG);
		exit(EXIT_FAILURE);
	}

	int threadPoolSize = atoi(argv[1]);

	if (threadPoolSize < 1) {
		fprintf(stderr, "%s\n", USAGE_ERR_MSG);
		exit(EXIT_FAILURE);
	}

	bool suppressSentMsg;

	if (argc >= 3) {
		suppressSentMsg = strcmp(argv[2], "-n");
	} else {
		suppressSentMsg = false;
	}

	// Create the message queue and or get its ID.
	key_t key = ftok("jrcleghorn", 0);
	int msgQueueId = msgget(key, IPC_CREAT | 0666);

	pthread_t threadIds[threadPoolSize];

	// Create mutexes for numMsgsReceived and numMsgsSent.
	pthread_mutex_t numMsgsReceivedLock;
	pthread_mutex_t numMsgsSentLock;

	// Initialize the mutexes.
	if (pthread_mutex_init(&numMsgsReceivedLock, NULL) != 0 || pthread_mutex_init(&numMsgsSentLock, NULL) != 0) {
		fprintf(stderr, "%s\n", MUTEX_INIT_ERR_MSG);
		exit(EXIT_FAILURE);
	}

	// Create the argument for the computeDotProduct function.
	ComputeDotProductArgument arg = {msgQueueId, suppressSentMsg, &numMsgsReceivedLock, &numMsgsReceived, &numMsgsSentLock, &numMsgsSent};

	// Create the computing threads.
	for (int i = 0; i < threadPoolSize; ++i) {
		pthread_create(&threadIds[i], NULL, computeDotProduct, &arg);
	}

	// Prevent termination.
	pthread_join(threadIds[0], NULL);

	return EXIT_SUCCESS;
}

void handleSigint(int signal) {
	printf("Jobs Sent %d Jobs Received %d\n", numMsgsSent, numMsgsReceived);
	return;
}
