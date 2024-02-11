use quinn::{ClientConfigBuilder, Endpoint, TransportConfig, Certificate, PrivateKey};
use std::net::SocketAddr;
use std::error::Error;
use tokio::net::UdpSocket;
use std::io::Write;

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    // Set up the address to connect to
    let addr = "127.0.0.1:5000".parse().expect("Failed to parse address");

    // Create QUIC endpoint
    let mut endpoint_builder = Endpoint::builder();
    endpoint_builder.default_client_config(make_client_config());

    // Build the QUIC endpoint
    let (endpoint, _) = endpoint_builder.bind(&"[::]:0".parse()?)?;

    // Connect to the server
    let quic_conn = endpoint.connect(&addr, "localhost").await?;

    // Open a stream
    let (send, mut recv) = quic_conn
        .open_bi()
        .await
        .expect("Failed to open bidirectional stream");

    // Send an integer to be echoed back
    let int_value = 42;
    let bytes = int_value.to_be_bytes();
    send.write_all(&bytes).await.expect("Failed to write to stream");

    // Read the echoed integer from the server
    let mut buf = [0; 4];
    recv.read_exact(&mut buf).await.expect("Failed to read from stream");
    let echoed_value = i32::from_be_bytes(buf);

    println!("Echoed value from server: {}", echoed_value);

    Ok(())
}

fn make_client_config() -> quinn::ClientConfig {
    let mut client_config = ClientConfigBuilder::default();

    let certificate = quinn::pem::parse_x509_pem(include_bytes!("cert.pem")).unwrap().0;
    let key = quinn::pem::parse_private_key_pem(include_bytes!("key.rsa")).unwrap();
    client_config.add_certificate_authority(Certificate::from_der(&certificate).unwrap()).unwrap();
    client_config.set_protocols(&[b"quic-client"[..]]);

    client_config.build()
}
