/* 
 * mm-handout.c - LIFO and Doubly linked list based on EXpilicit free lists and 
 *                 first fit placement (similar to lecture4.pptx). 
 *                 It does use boundary tags and does perform
 *                 coalescing. But still LIFO has some internal fragmentation 
 * 									which lower downs the performance then original lib malloc
 * Each block has a header of the form:
 * 
 *      31                     3  2  1  0 
 *      -----------------------------------
 *     |a/f s  s  s  s  ... s  s  s  0  0  a/f
 *      ----------------------------------- 
 * 
 * where s are the meaningful size bits and a/f is set 
 * iff the block is allocated. The list has the following form:
 *
 * begin                                                         end
 * heap                                                          heap  
 *  -----------------------------------------------------------------   
 * |  pad   | hdr(8:a) |   pad   | zero or more usr blks | hdr(8:a) |
 *  -----------------------------------------------------------------
 *    four  | prologue |  four   |                       | epilogue |
 *    bytes | block    |  bytes  |                       | block    |
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "mm.h"
#include "memlib.h"


/* Team structure */
team_t team = {
	/* Team name */
	" 3 idiots ",
	/* note that we will add a 10% bonus for
	* working alone */
	/* the maximum number of members per team
	 * is four */
	/* First member's full name */
	"Rahul Prajapati",
	/* First member's email address */
	"rprajapati@cse.unl.edu",
	/* Second member's full name (leave
	* blank if none) */
	"Akshat Goel",
	/* Second member's email address
	* (leave blank if none) */
	"agoel@cse.unl.edu",
	/*Thrid team memeber*/
	"Shivani Tamkiya",
	
	"stamkiya@cse.unl.edu"
};


/* $begin mallocmacros */
/* Basic constants and macros */

/* You can add more macros and constants in this section */
#define WSIZE       4       /* word size (bytes) */  
#define DSIZE       8       /* doubleword size (bytes) */
#define CHUNKSIZE  (1<<12)  /* initial heap size (bytes) */
#define OVERHEAD    4       /* overhead of header (bytes) */
#define FREEBLOCK 	1
#define ALLOCATE 		0

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(size_t *)(p))
#define PUT(p, val)  (*(size_t *)(p) = (val))  

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header*/
#define HDRP(bp)       ((char *)(bp) - WSIZE)  
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/*Given block ptr bp, compute address next free block*/
#define NEXT_FREEBLK(bp) (*(char **)(bp))
#define PREV_FREEBLK(bp)  (*(char **)(bp + WSIZE))

/* Given block ptr bp, compute address of next block */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char*)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))
/* $end mallocmacros */


/* Global variables */
static char *heap_listp;  /* pointer to first block */  
static char *free_block_listp;

/* function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void printblock(void *bp);
static void* check_coalesce(void *bp);
static void add_free_block( void *bp);
static void remove_free_block(void *bp);

/* Checked
 * mm_init - Initialize the memory manager 
 */
/* $begin mminit */
int mm_init(void) 
{
   /* create the initial empty heap */
   if ((heap_listp = mem_sbrk(4*WSIZE)) == NULL)
		return -1;
   PUT(heap_listp, KEY);               /* alignment padding */
   PUT(heap_listp+WSIZE, PACK(DSIZE, 0));  /* prologue header */ 
   PUT(heap_listp+DSIZE, PACK(DSIZE, 0));  /* footer word*/ 
   PUT(heap_listp+DSIZE+WSIZE, PACK(0, 0));   /* epilogue header */
   heap_listp += (DSIZE);

   //TODO: Set first free pointer to the starting of heap list
   free_block_listp = heap_listp;

   /* Extend the empty heap with a free block of CHUNKSIZE bytes */
   if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
		return -1;
   return (int) heap_listp;
}
/* $end mminit */

/* Checked
 * mm_malloc - Allocate a block with at least size bytes of payload 
 */
/* $begin mmmalloc */
void *mm_malloc(size_t size) 
{
    size_t asize;      /* adjusted block size */
    size_t extendsize; /* amount to extend heap if no fit */
    char *bp;      
	//	printf("call mm_malloc\n");

    /* Ignore spurious requests */
    if (size <= 0)
		return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= WSIZE)
		asize = WSIZE + (2 * OVERHEAD);
    else
		asize =  DSIZE + DSIZE * ((size + (OVERHEAD) + (DSIZE-1)) / DSIZE);
	 //printf("asize = %d\n", asize);
    
    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
		place(bp, asize);
		return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize,CHUNKSIZE);
	 //printf("extendsize = %d\n", extendsize);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
	 {
	 	printf("mm_malloc = NULL\n");
		return NULL;
	 }
	 //printf("return address = %p\n", bp);
    place(bp, asize);
	 //mm_checkheap(1);
    return bp;
} 
/* $end mmmalloc */

/* Checked
 * mm_free - Free a block 
 */
/* $begin mmfree */
void mm_free(void *bp)
{
	//printf("call mm_free\n");
		//1 is free and 0 is allocated
		/* You need to implement this function */
	if(bp != NULL)
	{
		
		//TODO: Setting current block to free 
		size_t current_block_size = GET_SIZE(HDRP(bp));
		PUT(HDRP(bp), PACK(current_block_size, FREEBLOCK));
		PUT(FTRP(bp), PACK(current_block_size, FREEBLOCK));
		check_coalesce(bp);		
	}
	return; 
}

/* We are checking the allocation bit of the next and previous block.
If even one of them is free, we are removing it from the free list and adding its size in the current block.
Wer are looping through the next and previous possible blocks till they are free. */

void* check_coalesce(void *bp)
{
  size_t current_size_block = GET_SIZE(HDRP(bp));
  size_t next_block_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t prev_block_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
    
	 //TODO: If both next and previous block 
  if(next_block_alloc == FREEBLOCK && prev_block_alloc == FREEBLOCK)
  {
			void* current_bp = bp;
			while( next_block_alloc != ALLOCATE)
			{
			current_size_block += GET_SIZE(HDRP(NEXT_BLKP(bp)));
			//Removing free block from list
			remove_free_block(NEXT_BLKP(bp));
			bp = NEXT_BLKP(bp);
			next_block_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
			}
			
			bp = current_bp;
			
			while( prev_block_alloc != ALLOCATE)
			{
			current_size_block += GET_SIZE(HDRP(PREV_BLKP(bp)));
			//Removing free block from list
			remove_free_block(PREV_BLKP(bp));
			bp = PREV_BLKP(bp);
			prev_block_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
			}
			
			PUT(HDRP(bp), PACK(current_size_block, FREEBLOCK));
			PUT(FTRP(bp), PACK(current_size_block, FREEBLOCK));
  }
	 //TODO: If only next block is free 
	else if(next_block_alloc == FREEBLOCK && prev_block_alloc == ALLOCATE)
  {
			void* current_bp = bp;
			while( next_block_alloc != ALLOCATE)
			{
			current_size_block += GET_SIZE(HDRP(NEXT_BLKP(bp)));
			//Removing free block from list
			remove_free_block(NEXT_BLKP(bp));
			bp = NEXT_BLKP(bp);
			next_block_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
			}
			bp = current_bp;
			PUT(HDRP(bp), PACK(current_size_block, FREEBLOCK));
			PUT(FTRP(bp), PACK(current_size_block, FREEBLOCK));
  }
    //TODO: If only previous block is free 
  else if(next_block_alloc == ALLOCATE && prev_block_alloc == FREEBLOCK)
	{
			while( prev_block_alloc != ALLOCATE)
			{
			current_size_block += GET_SIZE(HDRP(PREV_BLKP(bp)));
			//Removing free block from list
			remove_free_block(PREV_BLKP(bp));
			bp = PREV_BLKP(bp);
			prev_block_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
			}
			PUT(HDRP(bp), PACK(current_size_block, FREEBLOCK));
			PUT(FTRP(bp), PACK(current_size_block, FREEBLOCK));

  }
	//Adding new block in free list 
	add_free_block(bp);
	return bp;
}


//Checked
/*  Adding the free block to the free list to keep track of all the free blocks */
static void add_free_block(void *bp)
{
    if(bp != NULL)
    {
        NEXT_FREEBLK(bp) = free_block_listp;
        PREV_FREEBLK(bp) = NULL;
        if(free_block_listp != NULL)
        {
            PREV_FREEBLK(free_block_listp) = bp;
        }
        free_block_listp = bp;
    }
}

//Checked
/*  removing the free block from the free list to keep track of all the free blocks */
static void remove_free_block(void *bp)
{	
	if(bp != NULL )
	{
			
		//TODO: Check if block is allocated or not if its block return 
		if(GET_ALLOC(HDRP(bp)) == ALLOCATE)
			return;

		//TODO: Check if only next free block and set list
		if(NEXT_FREEBLK(bp) != NULL && PREV_FREEBLK(bp) == NULL)
		{
			free_block_listp = NEXT_FREEBLK(bp);
			PREV_FREEBLK(NEXT_FREEBLK(bp)) = NULL;
		}else if(NEXT_FREEBLK(bp) ==NULL && PREV_FREEBLK(bp) != NULL) 
		{
			//TODO: Check if only next free block and set list
			NEXT_FREEBLK(PREV_FREEBLK(bp)) = NULL;
		}else if(NEXT_FREEBLK(bp) != NULL && PREV_FREEBLK(bp) != NULL)
		{
			//TODO: Check if both left and right are free blocks
			NEXT_FREEBLK(PREV_FREEBLK(bp)) = NEXT_FREEBLK(bp);
			PREV_FREEBLK(NEXT_FREEBLK(bp)) = PREV_FREEBLK(bp);
		}
	
	}
}


/* $end mmfree */

/*Checked
 * mm_realloc - naive implementation of mm_realloc
 */
void *mm_realloc(void *ptr, size_t size)
{	
	//TODO: check if ptr is null then retrun mm_malloc 
	if(ptr == NULL)
	{
		return mm_malloc(size);
	}else
	{
		//TODO: Get current size of block pointer 
		size_t current_size = GET_SIZE(HDRP(ptr));
		
		//TODO: check if size is then ptr is ready to free
		if (size == 0)
		{
			mm_free(ptr);
			return 0;
		}
		
		//TODO: Ready to asign in heap 
		void *new_assign_ptr = mm_malloc(size);

		//TODO: check if current size is smaller then 
		if(current_size < size)
		{
			memcpy(new_assign_ptr, ptr, current_size);
			mm_free(ptr);
			return new_assign_ptr;
		}else
		{
			memcpy(new_assign_ptr, ptr, size);
			mm_free(ptr);
			return new_assign_ptr;

		}
	}
	return NULL;

}

/* Checked
 * mm_checkheap - Check the heap for consistency 
 */
void mm_checkheap(int verbose) 
{
	char *bp = heap_listp;

	if (verbose)
		printf("Heap (%p):\n", heap_listp);
	if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || GET_ALLOC(HDRP(heap_listp)))
		printf("Bad prologue header\n");

	for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
		if (verbose)
			printblock(bp);
	}

	if (verbose)
		printblock(bp);
	if ((GET_SIZE(HDRP(bp)) != 0) || (GET_ALLOC(HDRP(bp))))
		printf("Bad epilogue header\n");
}



/* The remaining routines are internal helper routines */

/* Checked
 * extend_heap - Extend heap with free block and return its block pointer
 */
/* $begin mmextendheap */
static void *extend_heap(size_t words) 
{
    char *bp;
    size_t size;
	
    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ((bp = mem_sbrk(size)) == (void *)-1) 
		return NULL;

    /* Initialize free block header and the epilogue header */
    PUT(HDRP(bp), PACK(size, FREEBLOCK));         
    PUT(FTRP(bp), PACK(size, FREEBLOCK));
    
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 0)); /* new epilogue header */
    return check_coalesce(bp);
}
/* $end mmextendheap */

/* Checked
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
/* $begin mmplace */
/* $begin mmplace-proto */
static void place(void *bp, size_t asize)
/* $end mmplace-proto */
{
    size_t csize = GET_SIZE(HDRP(bp));   
	// printf("csize = %d\n", csize);
    remove_free_block(bp);
   
   //TODO: Place for both header and footer
   if ((csize - asize) >= ((2 * OVERHEAD) + WSIZE)) 
   { 
		PUT(HDRP(bp), PACK(asize, ALLOCATE));
		PUT(FTRP(bp), PACK(asize, ALLOCATE));
		
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize-asize, FREEBLOCK));
		PUT(FTRP(bp), PACK(csize-asize, FREEBLOCK));
		add_free_block(bp);
    }
    else 
    { 
		PUT(HDRP(bp), PACK(csize, ALLOCATE));
		PUT(FTRP(bp), PACK(csize, ALLOCATE));
    }
}
/* $end mmplace */

/* Checked
 * find_fit - Find a fit for a block with asize bytes 
 */
static void *find_fit(size_t asize)
{

	if (free_block_listp == NULL){ return NULL; }
	void* bp;
    	/* first fit search */
			//TODO: Loop through free block  to find fit
    for(bp = free_block_listp; GET_ALLOC(HDRP(bp)) == FREEBLOCK; bp = NEXT_FREEBLK(bp))
		  {
			if (NEXT_FREEBLK(bp) != NULL)
			{
				//TODO: Check if size is fitting are smaller return block
				if((asize <= GET_SIZE(HDRP(bp)))) 
				{
					return bp;
				}
			}
			else {return NULL; }
			}
    return NULL; /* no fit */
}
//Checked
static void printblock(void *bp)
{
	  size_t header_size, header_alloc;
		header_size = GET_SIZE(HDRP(bp));
		header_alloc = GET_ALLOC(HDRP(bp));

		//TODO: Assign footer vaiables for size and alloc
    size_t footer_size, footer_alloc;
		footer_size = GET_SIZE(FTRP(bp));
		footer_alloc = GET_ALLOC(FTRP(bp));
		
		if (header_size == 0) {
			printf("%p: EOL\n", bp);
			return;
		}

		printf("%p: header: [%zu:%c]\n", bp, header_size, (header_alloc ? 'f' : 'a'));
		//Print footer
		printf("%p: footer: [%zu:%c]\n", bp, footer_size, (footer_alloc ? 'f' : 'a'));
}

