# Welcome to My Malloc
In this task we create our own implementation of malloc, calloc, realloc and free to allocate and free memory on-demand

## Task
Create your own implementation of the malloc functions in order to allocate memory

## Description
We created the following GcSTDLib functions but prefixed using `my_`:
```
malloc  -> my_malloc
free    -> my_free
calloc  -> my_calloc
realloc -> my_realloc
```

## Installation
If you don't want to re-run the project all the time just use:
```
make && ./my_malloc
```

If you DO want to re-run the compilation (useful for development) you can do:
```
make re && ./my_malloc
```

## Debug mode
If you want to run the my_malloc library in `DEBUG MODE` you must open [my_malloc.c](./my_malloc.c) (lines 10-14) and:
```c
/*
    If you want to see debug informations about allocations
    and/or memory addresses just uncomment the line bellow:
*/
//#define MY_MALLOC_DEBUG
```
And just uncomment line 14.


## Usage
It re-implements the GcSTDLib functions listed in [the description](#description).

If you are interested in low-level stuff (such as how do the memory allocations work and what addresses have been allocated) you can [turn debug mode ON](#debug). It is OFF by default.

The [main.c](./main.c) file provides a simple test environment for ALL of the implemented functions. You can check those out and/or provide your own if you'd like.

### The Core Team
- Petar Angelov [(Shannarra)](https://github.com/Shannarra)
- [Chris Clayton](https://github.com/Chrs-Clytn)