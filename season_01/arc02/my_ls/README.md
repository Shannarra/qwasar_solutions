# ls

## Task
Recreate the `ls` command used daily under linux.
The entries are sorted alphabetically by default.
Parameters: 
- `-a`: include entries whose name is starting with a dot `.`
- `-t`: sort entries by last time modified

## Description
This program works as expected, handling only the -a and -t flags.

## Installation
No need to install anything, see the bellow section on how to run.

## Usage
```sh
make && ./ls [-at] [path]
```
The code will understand and work correctly with 1, 2 or 3 parameters, has no memory leaks and works in __--pedantic__ mode.

## Qwasar Reference
My LS, season 01 Arc 02. Created by Petar Angelov, see meta section for more information.

#### Alternatively you can

```sh
gcc  -Wall -Wextra -Werror -g3 -fsanitize=address ls.c -o ls && ./ls
```
### Additional info for code
Next to the `#include`s there are comments with the functions used from the given header.  

The explaination for what does a function do are in a comment above it in [Rust DOCSTRINGS](https://doc.rust-lang.org/rust-by-example/meta/doc.html).

### Meta
Created by: [Petar Angelov](https://www.github.com/Shannarra).  
IDE used: [Emacs](https://www.emacs.org).
