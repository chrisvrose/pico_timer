use iced::{Element, widget::text};

use crate::{
    coms::App,
    messages::UIMessages::{self, Interact},
};

pub struct Msg;

pub fn update(app: &mut App, msg: UIMessages) {
    match msg {
        UIMessages::OPEN(_) => todo!(),
        Interact(pi_command) => {}
        UIMessages::ReqClose => todo!(),
    }
}

pub fn view(app: &App) -> Element<'_, UIMessages> {
    text!("text").into()
}
