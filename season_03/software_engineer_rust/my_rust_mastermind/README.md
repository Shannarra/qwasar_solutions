# Welcome to My Rust Mastermind

## Task
Recreate the "my Mastermind" game from Season01 Arc01

## Description
By creating a basic Cargo application using the `cargo new my_rust_mastermind` command.
All of the working code is in the [main.rs](./src/main.rs) file.

## Installation
You can just use 
```
cargo b
```

This will download all dependencies needed from the application, of which there is only one - the [rand crate](https://crates.io/crates/rand). It is used to generate a random number. 

## Usage
To use this you can use 2 different commands:

1. If you want to use it with default number of tries and a randomly-generated code you can just:
```sh
cargo r
```

2. If you want to run with a custom code or/and a custom code you can do:
```sh 
cargo b && ./target/debug/my_rust_mastermind -c 1234 [-t NUMBER_OF_TRIES]
```

### CLI semantics
The basic semantics of the command are:
```sh 
cargo b && ./target/debug/my_rust_mastermind [-c CODE] [-t NUMBER_OF_TRIES]
```


We accept `-c` AND `--code`
We accept `-t` AND `--tries`

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
