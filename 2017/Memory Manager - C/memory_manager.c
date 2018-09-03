/*
 * Binghamton CS 451/551 Project "Memory manager".
 * This file needs to be turned in.	
 */

//YOU MAY NEED TO READJUST HOW YOU DEAL WITH POINTER ADDRESSES!

#include "memory_manager.h"

static STRU_MEM_LIST * mem_pool = NULL;

/*
 * Print out the current status of the memory manager.
 * Reading this function may help you understand how the memory manager organizes the memory.
 * Do not change the implementation of this function. It will be used to help the grading.
 */
void mem_mngr_print_snapshot(void)
{
    STRU_MEM_LIST * mem_list = NULL;

    printf("============== Memory snapshot ===============\n");
    
    mem_list = mem_pool; // Get the first memory list
    while(NULL != mem_list)
    {
        STRU_MEM_BATCH * mem_batch = mem_list->first_batch; // Get the first mem batch from the list 

        printf("mem_list %p slot_size %d batch_count %d free_slot_bitmap %p\n", 
                   mem_list, mem_list->slot_size, mem_list->batch_count, mem_list->free_slots_bitmap);
        bitmap_print_bitmap(mem_list->free_slots_bitmap, mem_list->bitmap_size);

        while (NULL != mem_batch)
        {
            printf("\t mem_batch %p batch_mem %p\n", mem_batch, mem_batch->batch_mem);
            mem_batch = mem_batch->next_batch; // get next mem batch
        }

        mem_list = mem_list->next_list;
    }

    printf("==============================================\n");
}

/*
 * Initialize the memory manager.
 * You may add your code related to initialization here if there is any.
 */
void mem_mngr_init(void)
{
    mem_pool = NULL;
}

/*
 * Clean up the memory manager (e.g., release all the memory allocated)
 */
void mem_mngr_leave(void)
{
	/* Add your code here */
	//Have a toDelete var and a current var.
	//Have toDelete point to batch right before currentVar
	//free all batches on a list starting from the first to the last
	//then free bitmap
	
	struct _stru_mem_list * currentList = mem_pool;
	struct _stru_mem_list * prevList;
	struct _stru_mem_batch * currentBatch = mem_pool->first_batch;
	struct _stru_mem_batch * prevBatch;
	
	while(currentList != NULL) {
		while (currentBatch != NULL) {
			prevBatch = currentBatch;
			free(prevBatch->batch_mem);
			currentBatch = currentBatch->next_batch;
		}
		prevList = currentList;
		free(prevList->free_slots_bitmap);
	}
}

/*
 * Allocate a chunk of memory 	
 */
void * mem_mngr_alloc(size_t size) {
	int sizeAlloc = SLOT_ALLINED_SIZE((int)size);
	int firstBit;
	//Loop through Memory batch list. If there are no free batches that = sizeAlloc, request MEM_BATCH_SIZE slots from kernel, add to end of list.
	if (mem_pool == NULL) {
	//  -if the list is empty completely, initialize linked list, add mem_pool;
	    struct _stru_mem_list * firstList = (struct _stru_mem_list *) malloc(1 * sizeof(struct _stru_mem_list));
	    mem_mngr_new_slot_size(firstList, sizeAlloc);
	    mem_pool = firstList;
	    return mem_pool->first_batch->batch_mem;
	} else {
	    struct _stru_mem_list * currentList = mem_pool;
	    struct _stru_mem_list * prevList;
	    while (currentList != NULL) {
	        if (currentList->slot_size == sizeAlloc) {
	            firstBit = bitmap_find_first_bit(currentList->free_slots_bitmap, currentList->bitmap_size, 1);
	//If there is a free batch that = sizeAlloc, find use bitmap_find_first_bit to find first free bit. Clear it, then return the memory.
	            if (firstBit >= 0) {
	                bitmap_clear_bit(currentList->free_slots_bitmap, currentList->bitmap_size, firstBit);
	                //increment 0 by # of bits per byte * Batch Mem to get proper bytes
	                //For example: say first bit is 20 and batch count is 16, you want the second batch
	                //0 + 16 = 16 < 20 
	                //16 + 16 = 32 !<20 don't go into loop.
	                struct _stru_mem_batch * currentBatch = currentList->first_batch;
	                int leftovers = firstBit%BIT_PER_BYTE;
	                for (int i = 0; i < firstBit/MEM_BATCH_SLOT_COUNT; i = i+MEM_BATCH_SLOT_COUNT) {
	                    if (currentBatch->next_batch != NULL) {
	                        currentBatch = currentBatch->next_batch;
	                    } else {
	                        printf("ERROR: Out of bounds!\n");
	                        return NULL;
	                    }
	                }
	                char * workingPtr = (char *) currentBatch->batch_mem;
	                workingPtr = workingPtr + leftovers*currentList->slot_size;
	                return (void *) workingPtr;
	            } else if (firstBit == -1) { //request new batch
	                void * tester = mem_mngr_request_batch(currentList);
	                return tester;
	            }
	        }
	    	prevList = currentList;
	        currentList = currentList->next_list;
	    }
	    //Request new List;
	    struct _stru_mem_list * newList = (struct _stru_mem_list *) malloc(1 * sizeof(struct _stru_mem_list));
	    mem_mngr_new_slot_size(newList, sizeAlloc);
	    prevList->next_list = newList;
	    return newList->first_batch;
	}
	//Add new bitmap to the list, Clear first bit in the requested area, then return memory

	//0 is occupied!
}

//Adds a new slot size and adds new batch
void mem_mngr_new_slot_size(struct _stru_mem_list * block, int size) {;
    block->slot_size = size;
    block->batch_count = 1;
    block->free_slots_bitmap = (unsigned char*) malloc(((block->batch_count)*(MEM_BATCH_SLOT_COUNT/BIT_PER_BYTE)) * sizeof(unsigned char));
    block->bitmap_size = MEM_BATCH_SLOT_COUNT/BIT_PER_BYTE;
    for (int i = 0; i < block->bitmap_size; i++) {
    	block->free_slots_bitmap[i] = 0xFF;
    }
    
    block->first_batch = new _stru_mem_batch;
    block->first_batch->batch_mem = malloc((MEM_BATCH_SLOT_COUNT * size) * sizeof(struct _stru_mem_batch));
    block->first_batch->next_batch = NULL;
    
    
	int firstBit = bitmap_find_first_bit(block->free_slots_bitmap, block->bitmap_size, 1);
	bitmap_clear_bit(block->free_slots_bitmap, block->bitmap_size, firstBit);
    
    block->next_list = NULL;
}

//Adds a new batch onto an existing slot size, then returns that batch
void * mem_mngr_request_batch(struct _stru_mem_list * block) {
    struct _stru_mem_batch * newBatch = new _stru_mem_batch;
    newBatch->batch_mem = malloc((MEM_BATCH_SLOT_COUNT * block->slot_size) * sizeof(struct _stru_mem_batch));
    newBatch->next_batch = NULL;
    
    struct _stru_mem_batch * current = block->first_batch;
    if(current->next_batch == NULL) {
        current->next_batch = newBatch;
    } else {
        current = current->next_batch;
    }
    mem_mngr_increase_bitmap_size(block);
    int firstFreeBit = bitmap_find_first_bit(block->free_slots_bitmap, block->bitmap_size, 1);
    bitmap_clear_bit(block->free_slots_bitmap, block->bitmap_size, firstFreeBit);
    return newBatch->batch_mem;
}

void mem_mngr_increase_bitmap_size(struct _stru_mem_list * block) {
    //((block->batch_count+1)*(MEM_BATCH_SLOT_COUNT/8))
    //MEM_BATCH_SLOT_COUNT/8 = # of bytes that need to be added to bitmap when batch_count increases
    block->batch_count++;
    unsigned char * temp = (unsigned char*) malloc(((block->batch_count)*(MEM_BATCH_SLOT_COUNT/BIT_PER_BYTE)) * sizeof(unsigned char));
    int oldSize = block->bitmap_size;
    block->bitmap_size = (block->batch_count)*(MEM_BATCH_SLOT_COUNT/BIT_PER_BYTE);
    for (int i = 0; i < oldSize; i++){
        temp[i] = block->free_slots_bitmap[i];
    }
    for (int i = oldSize; i < block->bitmap_size; i++) {
        temp[i] = 0xFF;
    }
    free(block->free_slots_bitmap);
    block->free_slots_bitmap = temp;
}
/*
 * Free a chunk of memory pointed by ptr
 */
void mem_mngr_free(void * ptr)
{
	/* Add your code here */
	//loop through batches to find the pointer pointed to by ptr, storing the previous 
	//batch in a var.
	//if found, check to see if it points to anything else.
	//If so, set prv batch to point to the batch that the block to be freed is
	//Figure out which bitmaps that section corresponded to, then realloc
	//call free on pointer
	
	struct _stru_mem_list * currentList = mem_pool;
	struct _stru_mem_batch * currentBatch = mem_pool->first_batch;
	int bitmapStart = 0;
	int extra = 0;
	char * workingPtr;
	
	while(currentList != NULL) {
		while(currentBatch != NULL) {
			char * workingPtr = (char*)currentBatch->batch_mem;
			workingPtr = workingPtr + MEM_BATCH_SLOT_COUNT*currentList->slot_size;
			void * upperBound = (void *) workingPtr;
			if ((ptr >= currentBatch->batch_mem) && (ptr < upperBound)) {
				void * currentPtr = currentBatch->batch_mem;
				while ((((int*)currentPtr) < ((int*)upperBound)) && (((int*)currentPtr) != ((int*)ptr))) {
					extra++;
					workingPtr = (char *) currentPtr;
					workingPtr = workingPtr + currentList->slot_size;
					currentPtr = (void *) workingPtr;
				}
				if (currentPtr != ptr) {
					printf("ERROR: This pointer does not point to the starting address of any slot!\n");
					return;
				} else if (bitmap_bit_is_set(currentList->free_slots_bitmap, currentList->bitmap_size, bitmapStart + extra) != 0) {
					printf("ERROR: You either tried to free a pointer that already is free, or you got another error.\n");
					return;
				} else {
					bitmap_set_bit(currentList->free_slots_bitmap, currentList->bitmap_size, bitmapStart+extra);
					printf("Pointer freed successfully.\n");
					return;
				}
			}
			currentBatch = currentBatch->next_batch;
			bitmapStart = bitmapStart + MEM_BATCH_SLOT_COUNT/BIT_PER_BYTE;
		}
		currentList = currentList->next_list;
	}
	printf("ERROR: pointer not part of the list!\n");
}






