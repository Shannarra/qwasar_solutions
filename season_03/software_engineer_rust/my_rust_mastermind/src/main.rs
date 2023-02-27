use my_rust_mastermind::mastermind::{Game};

fn main() -> core::result::Result<(), String> {
    let mut game = Game::from_args(std::env::args().collect::<Vec<String>>())?;

    println!("Will you find the secret code?\n---");
    game.run_loop()
}
