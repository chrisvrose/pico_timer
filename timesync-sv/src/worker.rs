use std::{
    io::{BufRead, ErrorKind, Write},
    sync::mpsc::{Receiver, SyncSender},
    time::Duration,
};

use bufstream::BufStream;
use serialport::SerialPort;

use crate::messages::{UIMessages, WorkerMessages};

pub struct Worker {
    port_name: String,
    port: Option<BufStream<Box<dyn SerialPort>>>,
    receiver: Receiver<UIMessages>,
    sender: SyncSender<WorkerMessages>,
}

impl Worker {
    pub fn new(
        port_name: String,
        sender: SyncSender<WorkerMessages>,
        receiver: Receiver<UIMessages>,
    ) -> Worker {
        Worker {
            port_name,
            port: None,
            sender,
            receiver,
        }
    }

    pub fn open(&mut self) -> Result<(), std::io::Error> {
        let port = serialport::new(&self.port_name, 115200)
            .timeout(Duration::from_secs(5))
            .open()?;

        let buffered_stream = BufStream::new(port);
        self.port = Some(buffered_stream);
        Ok(())
    }

    pub fn interact_once(&mut self) {
        let buffered_stream = self.port.as_mut().expect("How did we not have a port?");
        let mut pi_output = String::with_capacity(32);

        let pi_output_size_res = buffered_stream.read_line(&mut pi_output);

        match pi_output_size_res {
            Err(e) if e.kind() == ErrorKind::TimedOut => {}
            Err(err) => {
                panic!("error {}", err);
            }
            Ok(_) => {}
        }

        let output = Worker::process_input(&pi_output);
        if let Some(output) = output {
            println!("Responding with output {}", output);
            buffered_stream
                .write(output.as_bytes())
                .expect("Failed to write");
            buffered_stream.flush().expect("Could not flush bufstream");
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
}
