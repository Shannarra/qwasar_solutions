# QWASAR SOLUTIONS
My solutions moving through the "Software Engineering" programme at ELU ([European Leadership University](elu.nl)) and solving tasks on [Qwasar Silicon Valley](https://qwasar.io/).

All of the solutions have been accepted and have 100% score from automatic Gandalf reviews and the respective human reviewers.

## Usage and contributing
You can use the code and all of the knowledge here freely, so long as you credit me or just leave a star on this repo to help other students ;)

If you want to contribute just create a PR, after a review and if it gets accepted your contribution will become a part of this repo.

## Contents
### [qwasar.sh](./qwasar.sh)
This is a file used to automatically bootstrap folders and files in the dreaded "Quest XX" tasks. With a single run of this file you will be able to create all of the folders needed and bootstrap them with a sample file.

Let's say that for example you have a quest that has 5 exersices (e.g. `ex00` to `ex04`). In order to use this script all you have to do is the following:
1. Copy the script file into your directory, name it something appropriate, for example `qwasar.sh`
2. Run the command using the command line:
```sh
sh ./qwasar.sh -n 4 -f main.c
```
> This will automatically create folders named ex00 to ex04 and place a sample `main.c` file inside. 
***
### [Season 01](./season_01/)
This folder is split between 2 separate subfolders - `Arc 01` and `Arc 02`.

#### [Arc 01](./season_01/arc01/)
Contents:  
[MyMastermind](./season_01/arc01/my_mastermind/)  
[MyNgram](./season_01/arc01/my_ngram/)  
[MyPrintf](./season_01/arc01/my_printf/)  


#### [Arc 02](./season_01/arc02/)
Contents:  
[MyLs](./season_01/arc02/my_ls/)  
[MyReadline](./season_01/arc02/my_readline/)  
[MyTar](./season_01/arc02/my_tar/)  

***
### [Season 02](./season_02/)  
Contents:  
[Bootcamp Ruby](./season_02/bootcamp_ruby/):  
- [MyHammingDna](./season_02/bootcamp_ruby/my_hamming_dna/)  
- [MyRomanNumeralsConverter](./season_02/bootcamp_ruby/my_roman_numerals_converter/)  

[MyAllocine](./season_02/my_allocine/)  
[MyBc](./season_02/my_bc/)  
[MyCurl](./season_02/my_curl/)  
[MyLibasm](./season_02/my_libasm/)  
[MyMalloc](./season_02/my_malloc/)  
[MyMouse](./season_02/my_mouse/)  
[MyCssIsEasy1](./season_02/my-css-is-easy-i/)  

***
### [Season03](./season_03/)
For season 03 I decided to get a break from C and ran the Rust course. 
There are my solutions:

1. [Software Engineer Rust](./season_03/software_engineer_rust/)
- [MyPingServer](./season_03/software_engineer_rust/my_ping_server/)
- [MyRedisClient](./season_03/software_engineer_rust/my_redis_client/)
- [MyRedisServer](./season_03/software_engineer_rust/my_redis_server/)
- [MyRustMastermind](./season_03/software_engineer_rust/my_rust_mastermind/)
- [MyStringRust](./season_03/software_engineer_rust/my_string_rust/)
- [MyTokio](./season_03/software_engineer_rust/my_tokio/)

If you have completed the C or Go course, please create a PR and let's build a comprehensive library for all courses's solutions! ;)

