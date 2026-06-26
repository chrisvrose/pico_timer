#pragma once

#include "comm/server.hh"
#include <functional>
#include <optional>
#include <pico/types.h>

enum Input { NO_INPUT, PRESSED_BUTTON, PRESSED_SECONDARY_BUTTON };

class InputManager {
  private:
    ComSerialServer &comSerialServer;
    void send_message(ServerMsg msg_type) const;
    std::optional<ClientResponse> try_recv() const;

  public:
    InputManager(ComSerialServer &comSerialServer)
        : comSerialServer(comSerialServer) {}
    inline Input poll_input() const { return NO_INPUT; }

    bool request_time_from_com(std::function<bool(datetime_t)>) const;
};
