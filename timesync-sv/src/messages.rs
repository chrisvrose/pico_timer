pub enum UIMessages {
    OPEN(String),
    Interact(PiCommand),
    ReqClose,
}

pub enum WorkerMessages {
    AckClose,
    InteractResponse(PiResponse),
    RstClose,
}

// TODO
#[derive(Debug, PartialEq, Eq)]
pub enum PiCommand {
    SyncTime,
}
// TODO
pub struct PiResponse;
