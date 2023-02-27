# Welcome to My Ping Server

## Task
Create a basic, robust client-server application.

## Description
Using basic Rust tools providing `TCP` abilities, mainly `std::net::TcpStream`.

## Installation
This is a CARGO project, no need to install anything. No dependencies either.

## Usage


To run the server you can:
```sh
cd ./src/server/

cargo r
```

> The server will run automatically on a default port, which is 42422.

To run the client, open a new terminal window and type the following:
```sh
cd ./src/client

cargo r
```

> The client will manage the creation and destruction of the TCP connection to the server on the port specified above.

### The Core Team
- [Petar (Shannarra) Angelov](https://www.github.com/Shannarra)

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
