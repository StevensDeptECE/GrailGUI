use quinn::{Endpoint, Incoming, Connection, TransportConfig, Certificate, PrivateKey, ServerConfig, ServerConfigBuilder, EndpointDriver};
use std::net::SocketAddr;
use tokio::net::UdpSocket;

#[tokio::main]
async fn main() {
    // Set up the address to listen on
    let addr = "0.0.0.0:5000".parse().expect("Failed to parse address");

    // Create QUIC endpoint
    let mut builder = Endpoint::builder();
    builder.listen(TransportConfig::default());
    let certificate = quinn::pem::parse_x509_pem(include_bytes!("cert.pem")).unwrap().0;
    let key = quinn::pem::parse_private_key_pem(include_bytes!("key.rsa")).unwrap();
    builder.identity(Certificate::from_der(&certificate).unwrap(), PrivateKey::from_der(&key).unwrap());

    // Build the QUIC endpoint
    let mut endpoint = builder.bind(&addr).expect("Failed to bind QUIC endpoint");

    // Accept incoming connections
    loop {
        let (conn, _) = endpoint.accept().await.expect("Error accepting connection");
        tokio::spawn(handle_connection(conn));
    }
}

async fn handle_connection(conn: Connection) {
    // Process requests from the client
    let mut incoming = conn
        .incoming()
        .expect("Failed to get incoming stream");

    while let Some(stream) = incoming.next().await {
        let stream = stream.expect("Failed to open stream");
        tokio::spawn(handle_stream(stream));
    }
}

async fn handle_stream(stream: quinn::RecvStream) {
    let mut buf = [0; 4]; // Assuming a 4-byte integer
    let len = stream
        .read(&mut buf)
        .await
        .expect("Failed to read from stream");

    // Convert the received bytes to an integer
    let int_value = i32::from_be_bytes(buf);

    // Echo the integer back to the client
    let written = stream
        .write_all(&int_value.to_be_bytes())
        .await
        .expect("Failed to write to stream");

    println!("Echoed back: {}", int_value);
}
