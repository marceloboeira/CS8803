use std::io;
use std::net::TcpListener;
use std::process::exit;

fn main() -> io::Result<()> {
    let listener = TcpListener::bind("0.0.0.0:8888")?;

    for _ in listener.incoming() {
        exit(0);
    }
    Ok(())
}
