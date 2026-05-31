use bufstream::BufStream;
use serialport::{SerialPort, SerialPortInfo, SerialPortType, UsbPortInfo};
use std::{
    io::{BufRead, ErrorKind, Write},
    time::Duration,
};

fn main() {
    println!("Hello, world!");
    let ports = serialport::available_ports().unwrap();
    let selected_ports = ports
        .iter()
        .filter(|port| is_rpi_pico_serial(port))
        .collect::<Vec<_>>();

    let selected_port = selected_ports.first().expect("Expected one pico!");

    let selected_port_name = selected_port.port_name.clone();
    println!("Opening Serial {}", selected_port_name);
    let port = serialport::new(selected_port_name, 115200)
        .timeout(Duration::from_secs(5))
        .open()
        .expect("Failed to open");

    interact(port);
}

fn interact(mut port: Box<dyn SerialPort>) -> ! {
    let mut buffered_stream = BufStream::new(port.as_mut());
    loop {
        let mut pi_output = String::with_capacity(32);

        let pi_output_size_res = buffered_stream.read_line(&mut pi_output);

        match pi_output_size_res {
            Err(e) if e.kind() == ErrorKind::TimedOut => {}
            Err(err) => {
                panic!("error {}", err);
            }
            Ok(_) => {}
        }

        let output = process_input(&pi_output);
        if let Some(output) = output {
            println!("Responding with output {}", output);
            buffered_stream
                .write(output.as_bytes())
                .expect("Failed to write");
            buffered_stream.flush().expect("Could not flush bufstream");
        }
        // println!("Read o/p from pi: {:?}", pi_output.into_bytes());
    }
}

fn process_input(input: &String) -> Option<String> {
    let trimmed = input.trim();
    match trimmed {
        "REQ_TIME" => {
            println!("Requested: Time!");
            let datetime = chrono::Local::now();
            Some(format!("ACK({})", datetime.format("%Y-%m-%dT%H:%M:%S")))
        }
        "ACK_TIME" => {
            println!("Set time successfully!");
            None
        }
        "ERR_TIME" => {
            println!("Failed to set time :(");
            None
        }
        "ERR_TMO" => {
            println!("Pico Timed out, reset");
            None
        }
        s if s.is_empty() => None,
        s if s.starts_with("# ") => {
            println!("Debug: {}", s);
            None
        }
        s => {
            println!("Requested: Unknown command! ({})", s);
            None
        }
    }
}

fn is_rpi_pico_serial(port: &SerialPortInfo) -> bool {
    let port_type = &port.port_type;
    match port_type {
        SerialPortType::UsbPort(UsbPortInfo {
            vid: 0xca1f,
            pid: 0x51e9,
            ..
        }) => true,
        _ => false,
    }
}
