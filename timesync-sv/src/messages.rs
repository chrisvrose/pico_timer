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
pub struct PiCommand;
// TODO
pub struct PiResponse;
