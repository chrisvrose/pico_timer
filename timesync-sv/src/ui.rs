use iced::{Element, widget::text};

use crate::coms::App;

pub struct Msg;

pub fn update(app: &mut App, msg: Msg) {}

pub fn view(app: &App) -> Element<'_, Msg> {
    text!("text").into()
}
