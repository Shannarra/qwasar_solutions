# Welcome to My Libasm
How are you doing today? This project was the bane of my existence.

## Task
Reinventing the fucking wheel. 
This is consisting of creating some important STDC functions in x86_64 ASM.

## Description
Writing a lot of assembly and reading some docs from the [Linux manual pages](https://man7.org/linux/man-pages/man3/)

## Installation
If you don't wanna re-build every time and just want to test everything:
```
make && ./my_libasm
```

If you, however, want to do some development, maybe overwrite the tests in C, you can do:
```
make re && ./my_libasm
```

## Usage
How does it work?
It's basically doing some [dynamic linking with object files](https://medium.com/@bdov_/https-medium-com-bdov-c-dynamic-libraries-what-why-and-how-66cf777019a7) that are being generated from our assembly. 
So the ASM does the actual heavy lifting.

## Testing
I have provided a "testing library" in `main.c`, you can overwrite the test functions to your heart's desires.

### The Core Team
- Petar Angelov [(Shannarra)](https://github.com/Shannarra)
- [Chris Clayton](https://github.com/Chrs-Clytn)

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
