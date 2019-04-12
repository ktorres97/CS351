#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "msg.h"    /* For the message struct */
#include <iostream>
using namespace std;

/* The size of the shared memory chunk */
#define SHARED_MEMORY_CHUNK_SIZE 1000

/* The ids for the shared memory segment and the message queue */
int shmid, msqid;

/* The pointer to the shared memory */
void *sharedMemPtr;

/* The name of the received file */
const char recvFileName[] = "recvfile";


/**
 * Sets up the shared memory segment and message queue
 * @param shmid - the id of the allocated shared memory
 * @param msqid - the id of the shared memory
 * @param sharedMemPtr - the pointer to the shared memory
 */

message sndMsg, rcvMsg;

void init(int& shmid, int& msqid, void*& sharedMemPtr)
{

	/* TODO: 1. Create a file called keyfile.txt containing string "Hello world" (you may do
 		    so manually or from the code).
	         2. Use ftok("keyfile.txt", 'a') in order to generate the key.
		 3. Use the key in the TODO's below. Use the same key for the queue
		    and the shared memory segment. This also serves to illustrate the difference
		    between the key and the id used in message queues and shared memory. The id
		    for any System V object (i.e. message queues, shared memory, and sempahores)
		    is unique system-wide among all System V objects. Two objects, on the other hand,
		    may have the same key.
	 */
	cout << "\nGenrating Key" << endl;
	key_t key = ftok("keyfile.txt", 'a');
	if(key < 0){
		cerr << "FAILED: create key" << endl; exit(1);
	}
	else{
		cout << "PASS: created key" << endl;
	}


	/* TODO: Allocate a piece of shared memory. The size of the segment must be SHARED_MEMORY_CHUNK_SIZE. */
	cout << "\nAllocate piece of shared memory" << endl;
	shmid = shmget(key, SHARED_MEMORY_CHUNK_SIZE, 0666 | IPC_CREAT);
	if(shmid < 0){
		cerr << "FAILED: allocate piece of memory" << endl; exit(1);
	}
	else{
		cout << "PASS: allocate piece of memory" << endl;
	}


	/* TODO: Attach to the shared memory */
	cout << "\nAttach to shared memroy" << endl;
	sharedMemPtr = shmat(shmid, nullptr, 0);
	if(sharedMemPtr == (void*) - 1){
		cerr << "FAILED: attach to shared memory" << endl; exit(1);
	}
	else{
		cout << "PASS: attach to shared memory" << endl;
	}

	/* TODO: Create a message queue */
	/* Store the IDs and the pointer to the shared memory region in the corresponding parameters */
	cout << "\nCreate a message queue" << endl;
	msqid = msgget(key, 0666 | IPC_CREAT);
	if(msqid < 0)
	{
		cerr<< "FAILED: create the message queue."<<endl;
	}
	else
	{
		cout<<"PASS: created the message queue."<<endl;
	}
}


/**
 * The main loop
 */
void mainLoop()
{
	/* The size of the message */
	int msgSize = 1;

	/* Open the file for writing */
	FILE* fp = fopen(recvFileName, "w");

	/* Error checks */
	if(!fp)
	{
		perror("fopen");
		exit(-1);
	}

    /* TODO: Receive the message and get the message size. The message will
     * contain regular information. The message will be of SENDER_DATA_TYPE
     * (the macro SENDER_DATA_TYPE is defined in msg.h).  If the size field
     * of the message is not 0, then we copy that many bytes from the shared
     * memory region to the file. Otherwise, if 0, then we close the file and
     * exit.
     *
     * NOTE: the received file will always be saved into the file called
     * "recvfile"
     */

	/* Keep receiving until the sender set the size to 0, indicating that
 	 * there is no more data to send
 	 */

	while(msgSize != 0)
	{
		cout<<"\nIncoming new Message"<<endl;
		if(msgrcv(msqid, &rcvMsg, sizeof(rcvMsg), SENDER_DATA_TYPE, 0) < 0)
		{
			cerr << "FAILED: new message" << endl; exit(1);
		}
		else{
			cout << "PASSED: new message" << endl;
		}
		msgSize = rcvMsg.size;


		/* If the sender is not telling us that we are done, then get to work */
		if(msgSize != 0)
		{
			/* Save the shared memory to file */
			if(fwrite(sharedMemPtr, sizeof(char), msgSize, fp) < 0)
			{
				perror("fwrite");
			}

			/* TODO: Tell the sender that we are ready for the next file chunk.
 			 * I.e. send a message of type RECV_DONE_TYPE (the value of size field
 			 * does not matter in this case).
 			 */
			cout << "Hey sender, ready for the next file chunk." << endl;
			sndMsg.mtype = RECV_DONE_TYPE;
			sndMsg.size = 0;
			if(msgsnd(msqid, &sndMsg, 0, 0) < 0)
			{
				cerr<<"FAILED: send message"<<endl;
			}else{
				cout<<"PASS: sent message"<<endl;
			}
		}
		/* We are done */
		else
		{
			/* Close the file */
			fclose(fp);
		}
	}
}



/**
 * Perfoms the cleanup functions
 * @param sharedMemPtr - the pointer to the shared memory
 * @param shmid - the id of the shared memory segment
 * @param msqid - the id of the message queue
 */

void cleanUp(const int& shmid, const int& msqid, void* sharedMemPtr)
{
	/* TODO: Detach from shared memory */
	cout << "\nDetach from shared memory" << endl;
	if(shmdt(sharedMemPtr) < 0){
		cerr << "FAILED: detach from shared memory" << endl;
	}
	else{
		cout << "PASS: detached from shared memory" << endl;
	}


	/* TODO: Deallocate the shared memory chunk */
	cout << "\nDeallocate the shared memory chunk" << endl;
	if(shmctl(shmid, IPC_RMID, NULL) < 0){
		cerr << "FAILED: deallocate the shared memory chunk" << endl;
	}
	else{
		cout << "PASS: deallocated the shared memory chunk" << endl;
	}


	/* TODO: Deallocate the message queue */
	cout << "\nDeallocate the message queue" << endl;
	if(msgctl(msqid, IPC_RMID, NULL) < 0){
		cerr << "FAILED: deallocate the message queue" << endl;
	}
	else{
		cout << "PASS: deallocated the message queue" << endl;
	}
}

/**
 * Handles the exit signal
 * @param signal - the signal type
 */

void ctrlCSignal(int signal)
{
	/* Free system V resources */
	cleanUp(shmid, msqid, sharedMemPtr);
}

int main(int argc, char** argv)
{

	/* TODO: Install a singnal handler (see signaldemo.cpp sample file).
 	 * In a case user presses Ctrl-c your program should delete message
 	 * queues and shared memory before exiting. You may add the cleaning functionality
 	 * in ctrlCSignal().
 	 */
	signal(SIGINT,ctrlCSignal);

	/* Initialize */
	init(shmid, msqid, sharedMemPtr);

	/* Go to the main loop */
	mainLoop();

	/** TODO: Detach from shared memory segment, and deallocate shared memory and message queue (i.e. call cleanup) **/
	cleanUp(shmid, msqid, sharedMemPtr);


	return 0;
}
