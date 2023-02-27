pub struct Server {
    listener: std::net::TcpListener,
}

impl Server {
    fn construct_listener(port: i32) -> std::net::TcpListener {
        let listener_res = std::net::TcpListener::bind(format!("localhost:{}", port));

        if let Err(e) = listener_res {
            eprintln!(
                "[ERROR]: {} (address: localhost:{port})
HINT: Under Linux you can run \"lsof -ti tcp:{port} | xargs kill -9\" to free the given port",
                e,
            );
            std::process::exit(1);
        }

        listener_res.unwrap()
    }


    pub fn on_port(port: i32) -> Self {
        println!("redis_server started on localhost:{port}");

        let listener = Self::construct_listener(port);
        Self {
            listener
        }
    }

    pub fn handle_connections(self) {
        for stream in self.listener.incoming() {
            match stream {
                Ok(stream) => {
                    println!("New connection: {}", stream.peer_addr().unwrap());
                    std::thread::spawn(move || crate::util::handle_client(stream));
                }
                Err(e) => {
                    println!("[ERROR]: {}", e);
                }
            }
        }
        drop(self.listener);
    }
}
