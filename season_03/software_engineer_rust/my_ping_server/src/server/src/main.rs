use server::{
    server::Server,
    util::SERVER_DEFAULT_PORT
};

fn main() {
    let s = Server::on_port(SERVER_DEFAULT_PORT);
    s.handle_connections();
}
