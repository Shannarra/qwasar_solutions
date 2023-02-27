#![allow(unused_assignments)]
use my_redis_server::redis_engine;
use std::io::{Read, Write};

fn exec_redis_engine(executor: &redis_engine::Executor, command: &str, response: &mut String) {
    if executor.setup_properly {
        let resp = executor.exec(command.trim().to_string());
        if let Err(x) = resp {
            response.push_str(&x);
        } else {
            response.push_str(&resp.unwrap());
        }
    }
}

fn handle_client(mut stream: std::net::TcpStream) {
    let ctrl_c = std::sync::Arc::new(std::sync::atomic::AtomicBool::new(false));
    let ctrl_z = std::sync::Arc::new(std::sync::atomic::AtomicBool::new(false));
    signal_hook::flag::register(signal_hook::consts::SIGINT, std::sync::Arc::clone(&ctrl_c))
        .unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGTSTP, std::sync::Arc::clone(&ctrl_z))
        .unwrap();

    let executor = redis_engine::setup_executor(false);
    let mut sched = job_scheduler::JobScheduler::new();
    let mut clone = executor.clone();
    /*
    https://docs.rs/job_scheduler/latest/job_scheduler/
    sec   min   hour   day of month   month   day of week   year
     *     *     *      *              *       *             *
     */
    sched.add(job_scheduler::Job::new(
        "* 5 * * * *".parse().unwrap(),
        move || {
            clone.save();
        },
    ));

    let mut data = [0_u8; 500];

    // while we don't catch either CTRL+C or CTRL+Z
    while !ctrl_c.load(std::sync::atomic::Ordering::Relaxed)
        && !ctrl_z.load(std::sync::atomic::Ordering::Relaxed)
    {
        sched.tick();
        clone = executor.clone();

        {
            let mut idx = 0;

            for exp in unsafe { &redis_engine::EXPIRY_LIST } {
                if let Ok(elapsed) = exp.start.elapsed() {
                    if elapsed.as_secs() >= exp.wait_time {
                        executor.expire_value(&exp.key).unwrap();
                        if unsafe { redis_engine::EXPIRY_LIST.len() } > 0 {
                            unsafe {
                                redis_engine::EXPIRY_LIST.remove(idx);
                            }
                        }
                        idx += 1;
                    }
                }
            }
        }

        match stream.read(&mut data) {
            Ok(size) => {
                if let Ok(command) = String::from_utf8(data[0..size].to_vec()) {
                    if command.is_empty() {
                        continue;
                    }
                    let mut engine_response = String::new();
                    exec_redis_engine(&executor, &command, &mut engine_response);
                    assert_eq!(
                        stream.write(engine_response.as_bytes()).unwrap(),
                        engine_response.len()
                    );
                    executor.save();
                } else {
                    continue;
                };
            }
            Err(e) => {
                eprintln!("[ERROR]: {e}");
                break;
            }
        }
    }

    // got a SIGINT or a SIGTSTP here:
    executor.save();
    println!("Closing server...");
    std::process::exit(0);
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let mut port = String::from("6379"); //default port

    if args.len() >= 3 {
        let flag = &args[1];
        let _port = &args[2];

        if flag == "--port" {
            if let Ok(p) = _port.parse::<u32>() {
                port = p.to_string();
            } else {
                eprintln!(
                    "[ERROR]: Port must be a whole positive number! Starting on localhost:{port}"
                );
            }
        }
    }

    let listener_res = std::net::TcpListener::bind(format!("localhost:{}", port));

    if let Err(e) = listener_res {
        eprintln!("[ERROR]: {} (address: localhost:{port})
HINT: Under Linux you can run \"lsof -ti tcp:{port} | xargs kill -9\" to free the given port",
                  e,

        );

        return;
    }
    let listener = listener_res.unwrap();

    println!("redis_server started on localhost:{port}");

    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                println!("New connection: {}", stream.peer_addr().unwrap());
                std::thread::spawn(move || handle_client(stream));
            }
            Err(e) => {
                println!("[ERROR]: {}", e);
            }
        }
    }

    drop(listener);
}
