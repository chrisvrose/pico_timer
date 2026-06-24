use std::{sync::mpsc::sync_channel, thread};

use serialport::{SerialPortInfo, SerialPortType, UsbPortInfo};

use crate::{
    messages::{UIMessages, WorkerMessages},
    ui::{update, view},
    worker::Worker,
};
mod coms;
mod messages;
mod proto;
mod ui;
mod worker;
fn main() {
    println!("Hello, world!");
    let ports = serialport::available_ports().unwrap();
    let selected_ports = ports
        .iter()
        .filter(|port| is_rpi_pico_serial(port))
        .collect::<Vec<_>>();

    let selected_port = selected_ports.first().expect("Expected one pico!");

    let selected_port_name = selected_port.port_name.clone();

    // main-> worker
    let (main_sender, main_receiver) = sync_channel::<UIMessages>(10);

    let (worker_sender, worker_receiver) = sync_channel::<WorkerMessages>(10);

    iced::run(update, view).expect("What");
    // let (sender, receiver) = sync_channel::<WorkerMessages>(10);
    thread::scope(move |_scope| {
        println!("Ba");
        let mut worker = Worker::new(selected_port_name, worker_sender, main_receiver);
        worker.open().expect("It should work");

        loop {
            worker.interact_once();
        }
    });
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
