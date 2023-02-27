# my_printf

## HOW TO

### RUN:
```sh
make && ./my_printf
```

#### Alternatively you can

```sh
gcc  -Wall -Wextra -Werror -g3 -fsanitize=address my_printf.c -o my_printf && ./my_printf
```
### Used information
1. Wrapping integer into a string (`itoa`) - inspired by [this link](https://groups.google.com/g/comp.lang.c++.moderated/c/ihafayWmWU8?pli=1#00c4c8fcb24dd1cc)

### Additional info for code
Next to the `#include`s there are comments with the functions used from the given header.  

The explaination for what does a function do are in a comment above it in [Rust DOCSTRINGS](https://doc.rust-lang.org/rust-by-example/meta/doc.html).

### Meta
Created by: [Petar Angelov](https://www.github.com/Shannarra).  
IDE used: [Emacs](https://www.emacs.org).