use std::io::Write;

pub struct Client {
    stream: std::net::TcpStream,
    cmd: String,
}

impl Client {
    pub fn on_port(port: i32) -> Option<Self> {
        let stream = std::net::TcpStream::connect(format!("localhost:{port}"));
        let cmd = String::new();

        if let Ok(stream) = stream {
            return Some(Self {
                stream,
                cmd,
            })
        }

        None
    }

    pub fn read_command(&mut self) -> &mut Self {
        let stdin = std::io::stdin();
        stdin.read_line(&mut self.cmd).unwrap();
        self.cmd = self.cmd.trim().to_string();

        self
    }

    pub fn run(&mut self) {
        assert_eq!(self.stream.write(self.cmd.as_bytes()).unwrap(), self.cmd.len());

        crate::util::read_stream_response(&mut self.stream);
    }
}
