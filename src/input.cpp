#include <functional>
#include <input.hh>
#include <optional>
#include <pico/stdio.h>
#include <pico/time.h>
#include <pico/types.h>
#include <sys/select.h>
#include <sys/unistd.h>

std::optional<ClientResponse> InputManager::try_recv() const {
    return this->comSerialServer.try_recv();
}

void InputManager::send_message(ServerMsg msg_type) const {
    return this->comSerialServer.send(msg_type);
}

bool InputManager::request_time_from_com(std::function<bool(datetime_t)> callback) const {
    send_message(REQ_TIME);

    auto recvd = try_recv();
    if(recvd.has_value() && recvd.value().type==ClientResponseType::SET_TME){
        auto did_update = callback(recvd.value().time);
        send_message(did_update? ServerMsg::ACK_TIME:ServerMsg::ERR_TIME);
        return did_update;
    }
    return false;

}
