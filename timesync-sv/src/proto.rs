use std::io::{BufRead, ErrorKind, Read, Write};

use bufstream::BufStream;

pub struct SyncProtocolClient<T: Write + Read> {
    buf: BufStream<T>,
}

impl<T: Write + Read> SyncProtocolClient<T> {
    pub fn new(src: T) -> SyncProtocolClient<T> {
        let buf = BufStream::new(src);
        SyncProtocolClient { buf }
    }

    pub fn interact_once(&mut self) {
        let mut pi_output = String::with_capacity(32);

        let pi_output_size_res = self.buf.read_line(&mut pi_output);

        match pi_output_size_res {
            Err(e) if e.kind() == ErrorKind::TimedOut => {}
            Err(err) => {
                panic!("error {}", err);
            }
            Ok(_) => {}
        }

        let output = Self::process_input(&pi_output);
        if let Some(output) = output {
            println!("Responding with output {}", output);
            self.buf.write(output.as_bytes()).expect("Failed to write");
            self.buf.flush().expect("Could not flush bufstream");
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

    pub fn into_inner(self) -> BufStream<T> {
        self.buf
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::io::Cursor;

    #[test]
    fn can_construct_and_use_client() {
        let cursor = Cursor::new(Vec::<u8>::new());
        let mut client = SyncProtocolClient::new(cursor);
        assert!(client.buf.flush().is_ok())
    }

    #[test]
    fn can_send_message() {
        // TODO Bad test - refactor to use separate cursors
        let cursor: Cursor<_> = Cursor::new(Vec::from(r#"REQ_TIME"#.as_bytes()));
        let mut client = SyncProtocolClient::new(cursor);

        client.interact_once();

        assert!(client.buf.flush().is_ok());

        let cursor = client
            .into_inner()
            .into_inner()
            .expect("Could not get inner cursor");
        assert!(cursor.position() != 0);

        let inner_vec = cursor.into_inner();
        println!("Got: {}", String::from_utf8(inner_vec.clone()).unwrap());
        assert!(inner_vec.starts_with("REQ_TIMEACK(".as_bytes()));
        assert!(inner_vec.ends_with(")".as_bytes()));
    }
}
