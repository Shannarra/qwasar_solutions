# Welcome to My Tokio
***

## Task
Recreate the popular [Tokio](https://github.com/tokio-rs/tokio) library for rust without using any resources besides STD.

## Description
The problem was solved by creating a very simple "runtime" called [MyTokio](./src/my_tokio/mod.rs), holding references to internal [Spawner](./src/my_tokio/spawner.rs) and [Executor](./src/my_tokio/executor.rs). 

The Spawner creates simple [Task](./src/my_tokio/task.rs)s, which get queued by the runtime and once the Executor is run the queued `Future`s get executed.

## Installation
This is a Cargo project, so you can run it just by typing:
```sh
cargo r
```
This will download the dependencies, create and run a binary. If you just want to download dependencies and create the binary without running it you can do:
```sh
cargo b
```

## Usage
The project runs the simple [main](./src/main.rs) file given as an example for the task at hand. Nothing special.

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
