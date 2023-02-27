#![allow(unused_assignments)]
use std::io::{Read, Write};
use std::time::{SystemTime, UNIX_EPOCH};

pub const SERVER_DEFAULT_PORT: i32 = 42422;

pub fn get_epoch_ms() -> u128 {
    SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .as_millis()
}

pub fn handle_client(mut stream: std::net::TcpStream) {
    let mut data = [0_u8; 500];

    match stream.read(&mut data) {
        Ok(size) => {
            if let Ok(command) = String::from_utf8(data[0..size].to_vec()) {
                let mut response = String::new();
                if command.is_empty() {
                    response = "[ERROR]: You sent nothing.".to_string();
                } else {
                    match command.as_str() {
                        "ping" => {
                            response = format!("pong {time}", time=get_epoch_ms());
                        },
                        _ => {
                            response = "Unknown command.".to_string();
                        }
                    }
                }
                assert_eq!(stream.write(response.as_bytes()).unwrap(), response.len());
            } else {
                eprintln!("[ERROR]: Cannot read input");
            };
        }
        Err(e) => {
            eprintln!("[ERROR]: {e}");
        }
    }
}
