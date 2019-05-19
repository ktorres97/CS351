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
	int n; // process number
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

void readFile(string fileName, vector<Process>& processList)
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
		processList.push_back(p);
	}
	ifile.close();
}

void memoryMap(int& memSize, int& pageSize, vector<_block>& mb)
{
	_block b;
	for(int i = 0; i < (memSize/pageSize); i++)
	{
		b.begin = i*pageSize;
		b._end = (i+1)* pageSize - 1;
		b.n = -1;
		b.pageNumber = -1;
		b._free = true;
		mb.push_back(b);
	}
}

void removeFromMap(int &mSize, int& pSize, int& valueToRemove, vector<_block> &mp)
{
	for(int i = 0; i < mp.size(); i++)
	{
			if(mp[i].n == valueToRemove)
				mp[i]._free = true;
	}
}

bool addToMemoryMap(Process& p, vector<_block>& mp, int& mSize, int& pSize)
{//(p, mp, mSize, pSize

	double bs;
	int bsi;
	int s = -1;
	int freeSpaceCounter;
	int spaceValueUsed;
	bool added = false;
	int numPages = 1;

	for(int i = 0; i < p.numOfpieces; i++)
	{
		bs = p.memoryBlocks[i] / pSize;

		for(int j = i; j < p.numOfpieces; j++)
			bs += (p.memoryBlocks[i]/ pSize);
			bsi = (int) bs;

			for(int x = 0; x < mp.size(); x++)
			{
				if(mp[x]._free)
				freeSpaceCounter++;
				else
				freeSpaceCounter = 0;
				if(freeSpaceCounter == bsi)
				spaceValueUsed = i + 1 - bsi;
				else
				spaceValueUsed = -1;

			}

			if(spaceValueUsed == -1)
			{
				bsi = (int) bs;
				for(int x = 0; x < mp.size(); x++)
				{
					if(mp[x]._free)
					freeSpaceCounter++;
					else
					freeSpaceCounter = 0;
					if(freeSpaceCounter == bsi)
					spaceValueUsed = i + 1 - bsi;
					else
					spaceValueUsed = -1;
				}
			}

			if(spaceValueUsed > -1)
			{
					added = true;
					for(int g = spaceValueUsed; g < spaceValueUsed + ((p.memoryBlocks[g] + 99)/pSize); g++)
					{
						mp[g].n = p.id;
						mp[g]._free = false;
						mp[g].pageNumber = numPages;
						numPages++;
					}

			}

		}
}

void actualTime(vector<int>& timeList, vector<Process>& processL)
{
	bool _begin;
	bool _end;
	for(int i = 0; i < processL.size(); i++)
	{
		_begin = false;
		_end = false;
		for(int j = 0; j < timeList.size(); j++)
		{
			if(processL[i].arrivalTime == timeList[j])
				_begin = true;
			if(processL[i].arrivalTime + processL[i].lifeTime == timeList[j])
				_end = true;
		}
		if(!_begin)
		timeList.push_back(processL[i].arrivalTime);
		if(!_end)
		timeList.push_back(processL[i].lifeTime);
	}
	sort(timeList.begin(), timeList.end());
}

void displayResult(vector<int>& timeList, vector<Process>& pl,int &mSize, int &pSize)
{
	Process p;
	vector<int> q;
	vector<int> removeFromQ;
	vector<_block> mp;

//int memSize, int pageSize, vector<_block>& mb
	memoryMap(mSize, pSize, mp);

	for(int i = 0; i < timeList.size(); i++)
	{
		cout << "t = " << timeList[i] << ": ";

		for(int j = 0; j < pl.size(); j++)
		{
				if(pl[j].arrivalTime == timeList[i])
					{
							cout << " ";
							q.push_back(pl[j].id);
							cout << "Process " << pl[j].id << " arrives" << endl;
							cout << "        Input Queue: [ ";
							for(int x = 0; x < q.size(); x++)
								cout << q[x] << " ";
								cout << "]" << endl;
					}
				if(pl[j].arrivalTime + pl[j].lifeTime == timeList[i])
				{
						cout << " ";
						//int &mSize, int& pSize, int& valueToRemove, vector<_block> &mp
						cout << "Process " << pl[j].id << " completes" << endl;
						removeFromMap(mSize, pSize, pl[j].id, mp);


				}
		}

		while (q.size() != 0)
		{
				p = pl[q.front() - 1];
				if(addToMemoryMap(p, mp, mSize, pSize))
				{
						cout << "   MM moves Process " << p.id << " to memory" << endl;
						q.erase(q.begin());
						cout << "   Input Queue: [ ";
						for(int k = 0; k < q.size(); k++)
						cout << q[k] << " ";
						cout << "]" << endl;

						//Print memory map
						int mapCounter = 0 ;
						int s = 0;
						int e = 0;
						while(mapCounter < (mSize/pSize))
						{
							if(!mp[mapCounter]._free)
							{
								if(s!= 0)
								{
									e = mapCounter - 1;
									cout << "  " << s * pSize << "-" << ((e + 1) * pSize) - 1 << ": Free frames(s)" << endl;
									s = 0;
								}
								cout << mp[mapCounter].begin << "-" << mp[mapCounter]._end << ": Process "
								<< mp[mapCounter].n << " , Page " << mp[mapCounter].pageNumber << endl;
							}
								if( s == 0)
								s = mapCounter;

								mapCounter++;
						}

						if(s != 0)
						{
							e = mapCounter -1;
							cout << s * pSize << "-" << ((e + 1) * pSize) - 1 << ": Free frame(s)" << endl;
						}

				}

		}

	}
}

void displayMemoryMap( vector<_block> &mp, int mSize, int pSize)
{
	cout << "Memory Map:" << endl;


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
	vector<Process> processReadyQ;
	vector<_block> memoryList;
	vector<int> timeList;

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

	readFile(workLoad, processList);
	actualTime(timeList, processList);
	displayResult(timeList,processList, memSize, pageSize);

	return 0;
}
