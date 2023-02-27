use std::str::from_utf8;
use std::io::Read;

pub const SERVER_DEFAULT_PORT: i32 = 42422;

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
