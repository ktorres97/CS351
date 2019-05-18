#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

typedef struct _block{
	struct _block *next;
	size_t size;
	void * memoryAddress;
	bool	_free; //Is the block free
	size_t memorySize; //Size of memory block
	int begin; //Where the block starts
	int _end; //Where the  block ends
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

void readFile(string fileName, vector<Process>& processQ)
{
	Process p;
	int totalProcesses;
	vector<int> blockSizedValue; //Holds the sizes of each memory
	int blockValue;

	//Open file
	ifstream ifile;
	ifile.open(fileName);
	ifile >> totalProcesses;

	for(int i = 0; i < totalProcesses; i++)
	{
		ifile >> p.id;
		ifile >> p.arrivalTime;
		ifile >> p.lifeTime;
		ifile >> p.numOfpieces;

		for(int j = 0; j < p.numOfpieces; j++)
		{
				ifile >> blockValue;
				p.totalAddressSpace += blockValue;
				blockSizedValue.push_back(blockValue);
		}

		p.memoryBlocks = blockSizedValue;
		processQ.push_back(p);
	}
	ifile.close();
}

void memoryMap(int memSize, int pageSize, vector<_block>& mb)
{
	_block b;
	for(int i = 0; i < (memSize/pageSize); i++)
	{
		b.begin = i*pageSize;
		b._end = (i+1)* pageSize - 1;
		b.n = -1;
		b._free = true;
		mb.push_back(b);

	}

}
/*
void addToMemoryMap(Process p, vector<_block>& mb, vector<int> pages)
{
	for(int i = 0; i < pages.size(); i++)
	{
		for(int j = 0; j < mb.size(); j++)
		{
			if(pages[i] == mb[j].pageNumber)
			{
				mb[j]. = p.id;
				mb[j]._end = p.lifeTime;
				mb[j]._free = FALSE;
			}
		}

	}

}
*/
void actualTime(vector<int>& timeList, vector<Process>& processL)
{
	bool _begin = false;
	bool _end = false;
	for(int i = 0; i < processL.size(); i++)
	{
		for(int j = 0; j < timeList.size(); j++)
		{
			if(processL[i].arrivalTime == timeList[j])
				_begin = true;
			else if(processL[i].arrivalTime + processL[i].lifeTime == timeList[j])
				_end = true;
		}
		if(!_begin)
		timeList.push_back(processL[i].arrivalTime);
		else if(!_end)
		timeList.push_back(processL[i].lifeTime);
	}
	sort(timeList.begin(), timeList.end());
}

/*
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
		block->size = size;
		block->memoryAddress = memAdd+BLOCK_SIZE;
		return block;
	}
}

void freeMemBlock(_BLOCK **head){
	if(*head==NULL){}
	else{
		(*head)->_free = true;
	}
}
*/
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

	cout << "Enter the memory size in KBytes > 2000: ";
	cin >> memSize;
	cout << "Enter the page size(1:100, 2:200, 3:400) > 3: ";
	cin >> pageSize;

	switch (pageSize) {
		case 1: pageSize = 100;
			break;
		case 2: pageSize = 200;
			break;
		case 3: pageSize = 400;
			break;
	}

	cout << "Enter the Work Load File name: ";
	cin >> workLoad;

	readFile(workLoad, processQ);
	memoryMap(memSize, pageSize, memoryList);

	return 0;
}
