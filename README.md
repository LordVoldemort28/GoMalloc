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

    * `mm_malloc`: The 