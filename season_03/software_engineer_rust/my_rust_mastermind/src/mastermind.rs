use std::io::Write;

use crate::utils;

pub struct Game {
    code: String,
    tries: u32,
    round: u32,
    well: u32,
    miss: u32,
    guess: String,
}

impl Game {
    fn clear_guess(&mut self, message: String) {
        self.guess = "".to_string();
        print!("{message}");
        std::io::stdout().flush().unwrap();
    }

    fn store_input(&mut self, stdin: &std::io::Stdin) {
        stdin.read_line(&mut self.guess).unwrap();
        self.guess = self.guess.trim().to_string();
        self.well = 0;
        self.miss = 0;
    }

    fn repl(&mut self, stdin: &std::io::Stdin) -> &mut Self {
        self.clear_guess(format!("Round {}\n>", self.round));
        self.round += 1;

        self.store_input(stdin);

        while !utils::code_is_legal(&self.guess) {
            self.clear_guess("Wrong input!\n>".to_string());
            self.store_input(stdin);
        }

        std::io::stdout().flush().unwrap();
        self
    }

    fn check_input(&mut self) -> &mut Self {
        for idx in 0..self.code.len() {
            if self.code.contains(self.guess.chars().nth(idx).unwrap()) {
                if self.code.chars().nth(idx).unwrap() ==
                    self.guess.chars().nth(idx).unwrap() {
                        self.well += 1;
                    } else {
                        self.miss += 1;
                    }
            }
        }

        self
    }

    fn give_feedback(&mut self) {
        println!("Well placed pieces: {} \nMisplaced pieces: {}\n---", self.well, self.miss);
    }

    pub fn from_args(args: Vec<String>) ->  core::result::Result<Game, &'static str> {
        let mut code = String::new();
        let mut tries = 10;

        for idx in 1..args.len() {
            match args[idx].as_str() {
                "-c" | "--code" => {
                    utils::parse_code(&args, idx, &mut code)?;
                },
                "-t" | "--tries" => {
                    utils::parse_tries(&args, idx, &mut tries)?;
                },
                _ => {}
            }
        }

        if code.is_empty() {
            code = utils::generate_code();
        }

        Ok(Self { code, tries, round: 0, well: 0, miss: 0, guess: String::new() })
    }

    pub fn run_loop(&mut self) -> core::result::Result<(), String> {
        let stdin = std::io::stdin();

        while self.guess != self.code && self.round <= self.tries {
            self
                .repl(&stdin)
                .check_input()
                .give_feedback();

            if self.round == self.tries && self.well < 4 {
                return Err(format!("Number of tries exceeded! The correct code was \"{}\"", self.code));
            }
        }

        println!("Congratz! You did it!");
        Ok(())
    }
}
