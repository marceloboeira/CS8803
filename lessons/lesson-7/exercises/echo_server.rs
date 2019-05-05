use std::io;
use std::io::{Read, Write};
use std::net::{TcpListener};
use std::process::exit;

fn main() -> io::Result<()> {
    let listener = TcpListener::bind("0.0.0.0:8888")?;

    for stream in listener.incoming() {
        match stream {
            Ok(mut stream) => {
                let mut buffer = [0 as u8; 1024];
                match stream.read(&mut buffer) {
                    Ok(_) => {
                        let response = String::from_utf8(buffer.to_vec()).unwrap();
                        stream.write(response.to_uppercase().as_bytes()).unwrap();
                        exit(0);
                    }
                    Err(err) => {
                        panic!(err);
                    }
                }
            },
            Err(_) => {
                println!("Error");
            }
        }
    }

    Ok(())
}
