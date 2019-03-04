GoMalloc
-----------

Its a homework assignment to design our own personal
malloc() and free() method in C.

The `malloc-assignment.pdf` have detail plan
for this assignment.

Notes
-------
__General__
* The file to modify and handing in is `mm.c`.
* The `mdriver.c` program is a driver program that will
evaluate the correctness and performance of the program.
* Use the command `make` to generate the driver code and run
it command `./mdriver -V` where `-V` flag displays helpful
summary information. While `./mdriver -lV` also report the performance
of the dynammic memory management routines from standard C library(glibc)

__Instuction__
* The dynammic store allocator functions are defined in `mm.c` and `mm.h`

```c
int mm_init(void);
void *mm_malloc(size_t size);
void mm_free(void *ptr);
void *mm_realloc(void *ptr, size_t size);
```

* The current implementation in `mm.c` is simple
implicit list( One way traversal ) with no boundary tags
* We only implement `mm_init` and `mm_malloc` and latter
is working but poorly utilizing memory and operating slowly
because of unimplemented `mm_realloc`.

* First, implement `mm_free` and `mm_realloc`
    * `mm_init`: Is a function to perform necessary intialization
    , such as allocating the heap area. The function should return -1
    if some problem occurs otherwise starting address of heap

    * `mm_malloc`: The routine returns a pointer to an allocated block payload of least `size` bytes. The entire allocated block should lie within the heap region and should not overlap with any other allocated chunck.
    <br /> Your malloc implementation should do likewise and always return 8 bytes aligned pointer. __Where `mdriver` program tests from for 8 bytes alignment and terminates if the alignements check fails.__
    * `mm_free`: 
        * The `mm_free` routine frees the block pointed to by `ptr`.
        * It returns nothing. 
        * This routine is only guaranteed to work correctly when the passed pointer(`ptr`) was returned by an earlier call to `mm_malloc` or `mm_realloc` and has not yet been freed.
    * `mm_realloc`: Routine return a pointer to an allocated region of at least size bytes with the following:-
        * If `ptr` us NULL, the call is equivalent to `mm_malloc( size )`
        * If `size` is equal to zero, the call is equivalent to `mm_free( ptr )`
        * If `ptr` is not NULL, it must have been returned by an earlier call to `mm_malloc` or `mm_relloc`.
            * The call to `mm_relloc` changes the size of memory block pointed to by `ptr`( the old block ) to `size` bytes and returns the address of the new block.
    * For any trouble check `man` page of any function.

* __Heap Consistency Checker Should Inculde Function__
    * Is every block in the free list marked as free?
    * Are there any contiguous free bloacks that somehow escaped coalescing
    * Is every free block actually in the free list 
    * Do the pointers in the free list point to valid free blocks?
    * Do any allocated blocks overlap?
    * Do the pointers in a heap block point to valid heap addresses?

* __Support Routines__
    * `memlib.c` contains function like
        * `void mem_init( void )` Which initialize the memory system model 
        * `void mem_deinit( void )` free the storage used by the memory system model
        * `void mem_reset_brk()` reset the simulated brk pointer to make an empty heap
        * `void *mem_sbrk( int incr )`Expands the heap by `incr` bytes, where `incr` is a positive non-zero integers and returns a generic pointer to the first byte of the newly allocated heap area. The semantics are identical to the unix `sbrk` function, except that `mem_sbrk` accepts only a positive non-zero integer argument.<br />simple model of the sbrk function. Extends the heap by incr bytes and returns the start address of the new area. In this model, the heap cannot be shrunk.
        * `void *mem_heap_lo( void )` returns address of first byte in the heap
        * `void *mem_heap_hi( void )` returns a address of the lasy byte in the heap
        * `size_t mem_heapsize( void ) ` returns the current size of heap in the bytes
        * `size_t mem_pagesize( void )` returns the page size of the system
    * __Trace driven driver program flags__
        * `-t < tracedir >`: Looks for the default trace files in directory `tracedir` instead of the default directory defined in `config.h`
        * `-f < tracefile >`: Use one partcular `tracefile` from testing instead of the default set of tracefiles.
        * `-h`: Print the summary of the command line arguements 
        * `-l`: Run and measure `libc` malloc in addition to the students's malloc package
        * `-v`: Verbose output. Print a performance breakdown from each tracefile in a compact table. 
        * `-V`: More verbose output. Prints additional diagnostic information useful for debugging.
* __Implicit List system__
    * The system must not use boundary tags. Traversing the list can only go in one direction
    * You cannot change `mm-init` and `mm_malloc`. Simply add code to complete `mm_free` and `mm_realloc`.
    * `mm_realloc` must support all conditions stated as part of the rellocation semantics.
    * `mm_free` must support coalescing. Without it, your system will not pass all 11 test.
* __Explicit List System__
    * Add boundary tags to the intial block in `mm_init`. 
    * Choose a criterion to insert a free block into the list. The criterion should not affects correctness but can yield better performance for your system. This can lead to more performance points 
    * `mm_realloc` implementation must follow the previously stated semantics.
    * `mm_free` must support coalescing.

* __Hints__
    * Use the `mdriver -f` option. During initial development, using tiny trace files will simplify debug- ging and testing. We have included two such trace files (`short1,2-bal.rep`) that you can use for initial debugging.
    * Use the `mdriver -v and -V `options. The -v option will give you a detailed summary for each trace file. The -V will also indicate when each trace file is read, which will help you isolate errors.
    * Encapsulate your pointer arithmetic in C preprocessor macros.
    * __Stages__
        * First 9 trace contains request to `malloc` and `free `
        * Last 2 contains request for `realloc`,`malloc` and `free `
    
Implementation
-------------

* __Malloc__
* __Free__
    * 
* __Realloc__
