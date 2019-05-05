use std::io;
use std::net::TcpStream;
use std::process::exit;

fn main() -> io::Result<()> {
    let _ = TcpStream::connect("0.0.0.0:8888")?;

    exit(0);
}
