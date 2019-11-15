#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

#include "constants.h"
#include "structs/Matrix.h"
#include "functions/packageDotProduct.h"
#include "functions/processDotProduct.h"

// Use global variables so that the information is available to the signal handler.
int numMsgsSent = 0;
int numMsgsReceived = 0;

void handleSigint(int signal);
void printFileErrorMessage(char filename[]);

int main(int argc, char* argv[]) {
	signal(SIGINT, handleSigint);

	if (argc < 5) {
		fprintf(stderr, "usage: package input_matrix_file_1 input_matrix_file_2 output_matrix_file seconds_between_thread_creation\n");
		exit(EXIT_FAILURE);
	}

	FILE* inputMatrixFiles[2];
	pthread_t createInputMatrixThreadIds[2];

	// Create the input matrix creation threads.
	for (int i = 0; i < 2; ++i) {
		// Open the input matrix file.
		inputMatrixFiles[i] = fopen(argv[i + 1], "r");

		if (inputMatrixFiles[i] == NULL) {
			printFileErrorMessage(argv[i + 1]);
			exit(EXIT_FAILURE);
		}

		// Create the thread.
		pthread_create(&createInputMatrixThreadIds[i], NULL, createMatrixFromFile, inputMatrixFiles[i]);
	}

	int secondsBetweenThreadCreation = atoi(argv[4]);

	Matrix* factors[2];

	// Join the input matrix creation threads.
	for (int i = 0; i < 2; ++i) {
		// Join the thread.
		pthread_join(createInputMatrixThreadIds[i], (void*)&factors[i]);

		// Close the input matrix file.
		fclose(inputMatrixFiles[i]);
	}

	// Create the message queue and or get its ID.
	key_t key = ftok("jrcleghorn", 0);
	int msgQueueId = msgget(key, IPC_CREAT | 0666);

	// Calculate the number of elements that the product matrix will have.
	int numProductElements = factors[0]->numRows * factors[1]->numCols;

	PackageDotProductArgument* packageDotProductArgs[numProductElements];

	// Create a mutex for numMsgsSent.
	pthread_mutex_t numMsgsSentLock;

	// Initialize the mutex.
	if (pthread_mutex_init(&numMsgsSentLock, NULL) != 0) {
		fprintf(stderr, "%s\n", MUTEX_INIT_ERR_MSG);
		exit(EXIT_FAILURE);
	}

	pthread_t packageDotProductThreadIds[numProductElements];

	// Create the packaging threads.
	for (int i = 0; i < factors[0]->numRows; ++i) {
		for (int j = 0; j < factors[1]->numCols; ++j) {
			int productElementNum = (i * factors[1]->numCols) + j;

			// Create the argument for the packageDotProduct function.
			packageDotProductArgs[productElementNum] = createPackageDotProductArgument(productElementNum, factors[0]->numCols, i, j, factors, msgQueueId, &numMsgsSentLock, &numMsgsSent);

			// Create the thread.
			pthread_create(&packageDotProductThreadIds[productElementNum], NULL, packageDotProduct, packageDotProductArgs[productElementNum]);

			sleep(secondsBetweenThreadCreation);
		}
	}

	// Create the product matrix.
	Matrix* product = createMatrix(factors[0]->numRows, factors[1]->numCols);

	pthread_t processDotProductThreadIds[numProductElements];

	// Create a mutex for numMsgsReceived.
	pthread_mutex_t numMsgsReceivedLock;

	// Initialize the mutex.
	if (pthread_mutex_init(&numMsgsReceivedLock, NULL) != 0) {
		fprintf(stderr, "%s\n", MUTEX_INIT_ERR_MSG);
		exit(EXIT_FAILURE);
	}

	// Create the argument for the processDotProduct function.
	ProcessDotProductArgument processDotProductArg = {msgQueueId, product, &numMsgsReceivedLock, &numMsgsReceived};

	// Create the processing threads.
	for (int i = 0; i < numProductElements; ++i) {
		// Create the thread.
		pthread_create(&processDotProductThreadIds[i], NULL, processDotProduct, &processDotProductArg);

		sleep(secondsBetweenThreadCreation);
	}

	// Join the packaging threads.
	for (int i = 0; i < numProductElements; ++i) {
		// Join the thread.
		pthread_join(packageDotProductThreadIds[i], NULL);

		// Destroy the argument that was created for the packageDotProduct function.
		destroyPackageDotProductArgument(packageDotProductArgs[i]);
	}

	// Destroy the mutex that was created for numMsgsSent.
	pthread_mutex_destroy(&numMsgsSentLock);

	// Destroy the input matrices.
	for (int i = 0; i < 2; ++i) {
		destroyMatrix(factors[i]);
	}

	// Join the processing threads.
	for (int i = 0; i < numProductElements; ++i) {
		pthread_join(processDotProductThreadIds[i], NULL);
	}

	// Destroy the mutex that was created for numMsgsReceived.
	pthread_mutex_destroy(&numMsgsReceivedLock);

	// Open the output matrix file.
	FILE* outputMatrixFile = fopen(argv[3], "w");

	if (outputMatrixFile == NULL) {
		printFileErrorMessage(argv[3]);
		exit(EXIT_FAILURE);
	}

	// Print the product matrix to the output matrix file.
	printMatrixToFile(product, outputMatrixFile);

	// Close the output matrix file.
	fclose(outputMatrixFile);

	// Destroy the product matrix.
	destroyMatrix(product);

	return EXIT_SUCCESS;
}

void handleSigint(int signal) {
	printf("Jobs Sent %d Jobs Received %d\n", numMsgsSent, numMsgsReceived);
	return;
}

void printFileErrorMessage(char filename[]) {
	fprintf(stderr, "error: the file \"%s\" could not be opened\n", filename);
	return;
}
