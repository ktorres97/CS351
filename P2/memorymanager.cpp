#include <stdio.h>
#inclue <fstream>

typedef struct _block{
	struct _block *next; //Pointer to next block
	bool	_free; //Is the block free
	size_t size; //Size of memory block
	void *memoryAddress; //Address of starting point
	int pageNumber;
	int n; //number of processes
}_BLOCK;

struct _workLoadInfo {
	int id;
	int timeSubitted;
	int lifeTime;
	int addressSpace;
	vector<workLoad> memoryList;
};

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
	int memSize;
	int pageSize;
	string workLoad;

	cout << "Enter the memory size in KBytes: "
	cin >> memSize;
	cout << "Enter the page size: "
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

	int n;
	int id;
	int timeSubitted;
	int lifeTime;
	int addressSpace;
	ifstream ifile;
	ifile.open("workLoad");
	ifile >> n;

	for(int i = 0; i < n; i++)
	{
		ifile >> id;
		ifile >> timeSubitted;
		ifile >> lifeTime;
		ifile >> addressSpace;

	}
	ifile.close();



	return 0;
}
