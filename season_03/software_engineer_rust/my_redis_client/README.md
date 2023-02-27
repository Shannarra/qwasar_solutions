# Welcome to My Redis Client
This is a client (front-end of sorts) for the task called [My Redis Server](https://upskill.us.qwasar.io/users/angelov_p/projects/my_redis_server)

## Task
The task is to create a client-side application that communicates with the server. See next point for more info.

## Description
The application can run in 2 different modes:
1. Single-run mode

This mode just executes the command by establishing a connection on a specified or default host and port (see [usage](#usage)),
recieving the output from the server and printing the result.

2. REPL mode

In this mode the command establishes a connection and runs on an infinite loop, reading input from the STDIN and passing it to the server,
then retrieving and printing the result.

## Installation
You can run 
```sh
cargo b
```
This will build the application. No need to run anything special.

## Usage
How the command works is described in the [description](#description) section.

The command can accept multiple arguments that determine in what mode it runs:
```sh
cargo b && ./target/debug/my_redis_cli [--host HOST_NAME] [-p PORT_ID] [command]
```

All of the arguments are optional. 
If you want to run in Single-Run mode you can run:
```sh
cargo b && ./target/debug/my_redis_cli --host localhost get name
cargo b && ./target/debug/my_redis_cli -p 6379 get name
cargo b && ./target/debug/my_redis_cli -h localhost -p 6379 get name
cargo b && ./target/debug/my_redis_cli get name
```

As `--host` and `--port` are optional, not providing them will run the app on the default values, being `localhost` and `6379` respectively.
> Not providing a value for `command` will run the app in REPL mode

REPL mode can also be run using:
```sh
cargo b && ./target/debug/my_redis_cli
cargo b && ./target/debug/my_redis_cli --host localhost
cargo b && ./target/debug/my_redis_cli --port 6379
cargo b && ./target/debug/my_redis_cli --host localhost --port 6379
```


### The Core Team
- [Petar (Shannarra) Angelov](https://www.github.com/Shannarra)

If you wanna see how I develped the backend app step-by-step see my repo at: https://github.com/Shannarra/redis-rs

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
