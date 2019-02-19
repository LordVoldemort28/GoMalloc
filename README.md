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

    