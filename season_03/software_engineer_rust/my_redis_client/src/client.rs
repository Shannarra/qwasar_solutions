use crate::util;
use std::io::Write;

#[derive(Debug)]
struct ConnectionDetails {
    pub(crate) host: String,
    pub(crate) port: String,
    pub(crate) repl: bool,
}

impl ConnectionDetails {
    pub fn new(host: &str, port: &str, repl: bool) -> Self {
        Self {
            host: host.to_string(),
            port: port.to_string(),
            repl
        }
    }
}

#[derive(Debug)]
pub struct Client {
    pub connection: Result<std::net::TcpStream, std::io::Error>,
    connection_details: ConnectionDetails,
    cmd: String,
}

impl Client {
    pub fn from_args(args: Vec<String>) -> Result<Self, String> {
        let mut host = String::from("localhost");
        let mut port = String::from("6379");
        let mut repl = false;
        let mut cmd = String::new();

        util::parse_args(args, &mut cmd, &mut host, &mut port, &mut repl);
        Client::try_create(&host, &port, repl, cmd)
    }

    pub fn try_create(host: &str, port: &str, repl: bool, cmd: String) -> Result<Self, String> {
        let connection = std::net::TcpStream::connect(format!("{host}:{port}"));

        if connection.is_ok() {
            Ok(Self {
                connection,
                connection_details: ConnectionDetails::new(host, port, repl),
                cmd,
            })
        } else {
            Err("asdasd".to_string())
        }
    }

    pub fn run(&mut self) {
        if !self.connection_details.repl {
            self.run_one_time()
        } else {
            self.run_repl()
        }
    }

    pub fn run_one_time(&mut self) {
        match self.connection {
            Ok(ref mut stream) => {
                if self.cmd.to_lowercase().trim() == "info" {

                    util::print_info(&self.connection_details.host, &self.connection_details.port);
                    return;
                }

                assert_eq!(stream.write(self.cmd.as_bytes()).unwrap(), self.cmd.len());
                util::read_stream_response(stream);
            },
            Err(ref e) => {
                println!("Failed to connect: {e}");
            }
        }
    }

    pub fn run_repl(&mut self) {
        loop {
            match self.connection {
                Ok(ref mut stream) => {
                    print!("{host}:{port}>", host=self.connection_details.host, port=self.connection_details.port,);
                    std::io::stdout().flush().unwrap();

                    let mut input = String::new();

                    util::read_user_input(&mut input);

                    if input.to_lowercase().trim() == "exit" || input.to_lowercase().trim() == "quit" {
                        break;
                    }

                    if input.to_lowercase().trim() == "info" {
                        util::print_info(&self.connection_details.host, &self.connection_details.port);
                        continue;
                    }

                    assert_eq!(stream.write(input.as_bytes()).unwrap(), input.len());

                    util::read_stream_response(stream);
                },
                Err(ref e) => {
                    println!("Failed to connect: {e}");
                }
            }
        }
    }
}
