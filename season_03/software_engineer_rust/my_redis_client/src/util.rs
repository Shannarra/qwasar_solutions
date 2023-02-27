use std::io::Read;
use std::str::from_utf8;

pub fn print_info(host: &str, port: &str) {
    println!("
My Redis Client \n
Version: 0.1.0 \n
Rust version: 1.61.0 \n
Rust edition: 2021 \n
Running on {host}:{port}");
}

pub fn parse_args(args: Vec<String>, cmd: &mut String, host: &mut String, port: &mut String, repl: &mut bool) {
    let mut was_prev_flag = false;
    if args.len() == 1 {
        *repl = true;
    } else {
        for i in 1..args.len() {
            match args[i].as_str() {
                "--host" | "-h" => {
                    if was_prev_flag {
                        println!("Invalid argument for {}", args[i-1]);
                        return;
                    }

                    was_prev_flag = true;
                    if let Some(v) = args.get(i+1) {
                        *host = v.to_string();
                        continue;
                    } else {
                        eprintln!("[ERROR]: No value provided for '--host', starting on default host 'localhost'");
                    }
                },
                "--port" | "-p" => {
                    if was_prev_flag {
                        println!("Invalid argument for {}", args[i-1]);
                        return;
                    }

                    was_prev_flag = true;
                    if let Some(v) = args.get(i+1) {
                        if v.parse::<u32>().is_ok() {
                            *port = v.to_string();
                            continue;
                        } else {
                            eprintln!("[ERROR]: Integer value required for --port");
                            return;
                        }
                    } else {
                        eprintln!("[ERROR]: No value provided for '--port', starting on default port '6379'");
                    }
                },
                _ => {
                    if !was_prev_flag {
                        cmd.push_str(&args[i]);
                        cmd.push(' ');
                    } else {
                        was_prev_flag = false;
                    }
                }
            }
        }
    }
}

pub fn read_stream_response(stream: &mut std::net::TcpStream) {
    let mut data = [0_u8; 50];

    match stream.read(&mut data) {
        Ok(_) => {
            if !data.is_empty() {
                println!("{}", String::from_utf8(data.to_vec()).unwrap());
            } else {
                let text = from_utf8(&data).unwrap();
                println!("Unexpected reply: {}", text);
            }
        },
        Err(e) => {
            println!("Failed to receive data: {}", e);
        }
    }
}

pub fn read_user_input(text: &mut String) {
    let stdin = std::io::stdin();
    stdin.read_line(text).unwrap();
}
