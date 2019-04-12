#include <sys/shm.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "msg.h"    /* For the message struct */
#include <fstream>
#include <iostream>
using namespace std;

/* The size of the shared memory chunk */
#define SHARED_MEMORY_CHUNK_SIZE 1000

/* The ids for the shared memory segment and the message queue */
int shmid, msqid;

/* The pointer to the shared memory */
void* sharedMemPtr;

/**
 * Sets up the shared memory segment and message queue
 * @param shmid - the id of the allocated shared memory
 * @param msqid - the id of the shared memory
 */

void init(int& shmid, int& msqid, void*& sharedMemPtr)
{
	/* TODO:
        1. Create a file called keyfile.txt containing string "Hello world" (you may do
 		    so manually or from the code).
	    2. Use ftok("keyfile.txt", 'a') in order to generate the key.
		3. Use the key in the TODO's below. Use the same key for the queue
		    and the shared memory segment. This also serves to illustrate the difference
		    between the key and the id used in message queues and shared memory. The id
		    for any System V objest (i.e. message queues, shared memory, and sempahores)
		    is unique system-wide among all SYstem V objects. Two objects, on the other hand,
		    may have the same key.
	 */
	key_t key = ftok("keyfile.txt", 'a');
	cout << "\nUse ftok(keyfile.txt, a) in order to generate the key" << endl;
	if(key < 0){
		cout << "FAILED: key for FTOK" << endl; exit(1);
	}
	else{
		cout << "PASS: created key for FTOK" << endl;
	}


	/* TODO: Get the id of the shared memory segment. The size of the segment must be SHARED_MEMORY_CHUNK_SIZE */
	shmid = shmget(key, SHARED_MEMORY_CHUNK_SIZE, IPC_CREAT | 0666);
	cout << "\nGet the id of the shared memory segment." << endl;
	if(key < 0){
		cout << "FAILED: get ID for shared memory" << endl; exit(1);
	}
	else{
		cout << "PASS: get ID for shared memory" << endl;
	}


	/* TODO: Attach to the shared memory */
	sharedMemPtr = shmat(shmid, nullptr, 0);
	cout << "\nAttach to the shared memory" << endl;
	if(sharedMemPtr == (void*)-1){
		cout << "FAILED: attach to shared memory" << endl; exit(1);
	}
	else{
		cout << "PASS: attach to shared memory" << endl;
	}


	/* TODO: Attach to the message queue */
	/* Store the IDs and the pointer to the shared memory region in the corresponding parameters */
	msqid = msgget(key, 0666 | IPC_CREAT);
	cout << "\nAttach to the message queue" << endl;
	if(msqid < 0){
		cout << "FAILED: attach to message queue." << endl;
	}
	else{
		cout << "PASS: attach to message queue." << endl;
	}
}

/**
 * Performs the cleanup functions
 * @param sharedMemPtr - the pointer to the shared memory
 * @param shmid - the id of the shared memory segment
 * @param msqid - the id of the message queue
 */

void cleanUp(const int& shmid, const int& msqid, void* sharedMemPtr)
{
	/* TODO: Detach from shared memory */
	shmdt(sharedMemPtr);
	cout << "\nDetaching from shared memory" << endl;
	if(shmdt(sharedMemPtr) < 0){
		cout << "FAILED: detach from the shared memory." << endl; exit(1);
	}
	else{
		cout << "PASS: detach from the shared memory." << endl;
	}
}

/**
 * The main send function
 * @param fileName - the name of the file
 */
void send(const char* fileName)
{
	/* Open the file for reading */
	FILE* fp = fopen(fileName, "r");

	/* A buffer to store message we will send to the receiver. */
	message sndMsg;

	/* A buffer to store message received from the receiver. */
	message rcvMsg;

	/* Was the file open? */
	if(!fp)
	{
		perror("fopen");
		exit(-1);
	}

	/* Read the whole file */
	while(!feof(fp))
	{
		/* Read at most SHARED_MEMORY_CHUNK_SIZE from the file and store them in shared memory.
 		 * fread will return how many bytes it has actually read (since the last chunk may be less
 		 * than SHARED_MEMORY_CHUNK_SIZE).
 		 */
		if((sndMsg.size = fread(sharedMemPtr, sizeof(char), SHARED_MEMORY_CHUNK_SIZE, fp)) < 0)
		{
			perror("fread");
			exit(-1);
		}


		/* TODO: Send a message to the receiver telling him that the data is ready
 		 * (message of type SENDER_DATA_TYPE)
 		 */
		cout << "\nSend a message to the receiver telling him that the data is ready" << endl;
		sndMsg.mtype = SENDER_DATA_TYPE;
		if (msgsnd(msqid, &sndMsg, sndMsg.size, 0) < 0){
			cout << "FAILED: send message, data not ready" << endl;
		}
		else{
			cout << "PASS: sent message, & ready the data is ready" << endl;
		}


		/* TODO: Wait until the receiver sends us a message of type RECV_DONE_TYPE telling us
 		 * that he finished saving the memory chunk.
 		 */
		cout << "\nWait until the receiver sends us a message of type RECV_DONE_TYPE" << endl;
		if (msgrcv(msqid, &rcvMsg, 0, RECV_DONE_TYPE, 0) < 0){
			cout << "FAILED: receive message" << endl; exit(1);
		}
		else{
			cout << "PASS: received message from sender" << endl;
		}
	}


	/** TODO: once we are out of the above loop, we have finished sending the file.
 	  * Lets tell the receiver that we have nothing more to send. We will do this by
 	  * sending a message of type SENDER_DATA_TYPE with size field set to 0.
	  */
	sndMsg.size = 0;
	sndMsg.mtype = SENDER_DATA_TYPE;
	cout << "\nsending a message of type SENDER_DATA_TYPE with size field set to 0" << endl;
	if(msgsnd(msqid, &sndMsg, sizeof(sndMsg), 0) < 0){
		cout << "FAILED: send blank message" << endl;
	}
	else
	{
		cout << "PASS: sent blank message." << endl;
	}

	/* Close the file */
	fclose(fp);
}


int main(int argc, char** argv)
{

	/* Check the command line arguments */
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <FILE NAME>\n", argv[0]);
		exit(-1);
	}

	/* Connect to shared memory and the message queue */
	init(shmid, msqid, sharedMemPtr);

	/* Send the file */
	send(argv[1]);

	/* Cleanup */
	cleanUp(shmid, msqid, sharedMemPtr);

	return 0;
}

