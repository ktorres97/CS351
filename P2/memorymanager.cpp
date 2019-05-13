#include <stdio.h>

typedef struct _block{
	struct _block *next; //Pointer to next block
	bool	_free; //Is the block free
	size_t size; //Size of memory block
	void *memoryAddress; //Address of starting point
}_BLOCK;

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

