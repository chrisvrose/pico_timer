#include <cstdint>
#include <cstring>
#include <optional>
#include <pico/types.h>
#include <string>

enum ServerMsg {
    REQ_TIME,
    ACK_TIME,
    ERR_TIME,
    /** Timeout */
    ERR_TMO
};

std::string serverMsgToString(ServerMsg serverMsg);

enum ClientResponseType { SET_TME, SET_ALM };

struct ClientResponse {
    ClientResponseType type;
    datetime_t time;
};

/**
 * Abstracts away the communication with the compooter.
 */
class ComSerialServer {
  private:
    static constexpr uint8_t RECV_BUFFER_LEN = 128;

    char recv_buffer[RECV_BUFFER_LEN] = {};

    uint8_t buffer_recv_current_len = 0;

    void clear_buffer() {
        memset(this->recv_buffer, 0, RECV_BUFFER_LEN);
        buffer_recv_current_len = 0;
    }

    std::optional<ClientResponse> try_parse_buffer();
  public:
    void send(ServerMsg msg_type) const;
    /**
     * Convenience logging function. Newlines get appended.
     */
    void log(std::string loggedMsg) const;

    /**
     *
     */
    std::optional<ClientResponse> try_recv();
};
