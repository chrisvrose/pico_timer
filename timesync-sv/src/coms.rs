#[derive(Default)]
pub struct App {
    state: AppState,
    error: Option<AppError>,
}

struct AppError;

#[derive(Debug, Default, Clone, Copy)]
enum AppState {
    #[default]
    Inactive,
    Active,
    Waiting,
    ActiveHadError,
}
