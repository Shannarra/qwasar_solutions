use my_redis_cli::client;

fn main() {
    let client = client::Client::from_args(std::env::args().collect::<Vec<String>>());
    client.expect("Could not create client").run();
}
