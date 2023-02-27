# readline

## Task
Recreate the `readline` function in C using file descriptors

## Description
This program works as expected, handling input of up to 500 characters by default reading a given file descriptor.

## Installation
No need to install anything, see the bellow section on how to run.

## Usage
```sh
make && ./my_readline
```
The code will work withouth any paramers, has no memory leaks and works in __--pedantic__ mode.  
If the given file name is invalid or the fd can't be read you will face an error.  

## Qwasar Reference
My readline, season 01 Arc 02. Created by Petar Angelov, see meta section for more information.

#### Alternatively you can

```sh
gcc  -Wall -Wextra -Werror -g3 -fsanitize=address main.c -o my_readline && ./my_readline
```
### Additional info for code
Next to the `#include`s there are comments with the functions used from the given header.  

The explaination for what does a function do are in a comment above it in [Rust DOCSTRINGS](https://doc.rust-lang.org/rust-by-example/meta/doc.html).

### Meta
Created by: [Petar Angelov](https://www.github.com/Shannarra).  
IDE used: [Emacs](https://www.emacs.org).
