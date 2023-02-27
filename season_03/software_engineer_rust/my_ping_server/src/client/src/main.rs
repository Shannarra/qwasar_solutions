use client::{
    client::Client,
    util::SERVER_DEFAULT_PORT
};

fn main() {
    let c = Client::on_port(SERVER_DEFAULT_PORT);
    c.expect("[ERROR]: Cannot create a client!").read_command().run();
}
