use std::{
    sync::mpsc::{Receiver, SyncSender},
    time::Duration,
};

use serialport::SerialPort;

use crate::{
    messages::{UIMessages, WorkerMessages},
    proto::SyncProtocolClient,
};

pub struct Worker {
    port_name: String,
    client: Option<SyncProtocolClient<Box<dyn SerialPort>>>,
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
            client: None,
            sender,
            receiver,
        }
    }

    pub fn open(&mut self) -> Result<(), std::io::Error> {
        let port = serialport::new(&self.port_name, 115200)
            .timeout(Duration::from_secs(5))
            .open()?;

        let client = SyncProtocolClient::new(port);
        self.client = Some(client);
        Ok(())
    }
}
