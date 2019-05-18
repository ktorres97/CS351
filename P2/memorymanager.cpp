#include <stdio.h>
#inclue <fstream>

typedef struct _block{
	bool	_free; //Is the block free
	size_t memorySize; //Size of memory block
	int begin; //Where the block starts
	int end; //Where the  block ends
	int pageNumber;
	int n; //number of processes
}_BLOCK;

struct Process {
	int id;
	int arrivalTime;
	int lifeTime;
	int numOfpieces; //The number of pieces of memory on each line
	int sizeOfMemoryPiece;
	int totalAddressSpace; //The total address space
	vector<int> memoryBlocks; //Holds the size of each memory block
};

void readFile(string fileName,int &numberOfProc, vector<Process>& processQ, vector<int>& list)
{
	Process p;
	ifstream ifile;
	ifile.open(fileName);
	ifile >> numberOfProc;

	for(int i = 0; i < numberOfProc; i++)
	{
		ifile >> p.id;
		ifile >> p.arrivalTime;
		ifile >> p.lifeTime;
		ifile >> p.numOfpieces;

		for(j = 0; j < p.numOfpieces; j++)
		{
				ifile >> sizeOfMemoryPiece;
				p.totalAddressSpace += sizeOfMemoryPiece;
				memoryBlocks.push_back(sizeOfMemoryPiece);
		}

		list.push_back(p.arrivalTime);
		list.push_back(p.lifeTime);
		processQ.push_back(p);
		p.totalAddressSpace = 0;
	}

}
#define BLOCK_SIZE sizeof(_BLOCK)

_BLOCK *allocateMemBlock(size_t size){
	_BLOCK *block = (_BLOCK)sbrk(0); //start at a break point
	void *memAdd = (void*)sbrk(0); //store address to memAdd
	void *allocate_mem = (void*)sbrk(BLOCK_SIZE + size); //Have room for all data

	if(allocate_mem == (void*)-1){ //End of file
		return NULL;
	}
	else{	//Make new block
		block->next = NULL;
		block->_free = false;
		block->size = size
		block->memoryAddress = memadr+BLOCK_SIZE;
		return block;
	}
}

void freeMemBlock(_BLOCK **head){
	if(*head==NULL){}
	else{
		(*head)->free = true;
	}
}

int main()
{
	int numberOfProcesses;
	int memSize;
	int pageSize;
	string workLoad;
	Process p;
	vector<Process> processList;
	vector<Process> processQ;
	vector<Process> processReadyQ;
	vector<_block> memoryList;
	vector<int> list;

	cout << "Enter the memory size in KBytes > 2000: "
	cin >> memSize;
	cout << "Enter the page size(1:100, 2:200, 3:400) > 3: "
	cin >> pageSize;

	switch (pageSize) {
		case 1: pageSize = 100;
			break;
		case 2: pageSize = 200;
			break;
		case 3: pageSize = 400;
			break;
	};

	cout << "Enter the Work Load File name: ";
	cin >> workLoad;

	readFile(workLoad, &numberOfProcesses, processQ, list);
	int n;

	ifile.close();



	return 0;
}
