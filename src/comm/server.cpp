#include <comm/server.hh>
#include <cstdio>
#include <iostream>
#include <optional>
#include <ostream>
#include <vector>
#include <pico/stdio.h>
#include <pico/time.h>
#include <pico/types.h>
#include <string>
/*
 * 1. It it a wrapper over serial.
 * 2. Use input.cpp and input.hh to see what requests and what responses it
 should have.
 * 3. input.cpp creates a new buffer each parse, but server.hh and server.cpp
 should use the class-level buffer.
 * 4. whenever it sends a reset, it should clear the buffer. whenever it gets a
 full command, it should clear the buffer. whenever it gets a full buffer, it
 should send a reset.

 */

/**
 * string interpretation of ServerMsg
 */
const std::vector<std::string> MESSAGE_TYPE_STRINGS = {"REQ_TME", "ACK_TME",
                                                       "ERR_TME", "ERR_TMO"};

std::string serverMsgToString(ServerMsg serverMsg) {
    return MESSAGE_TYPE_STRINGS[serverMsg];
}

void ComSerialServer::send(ServerMsg msg) const {
    std::cout << serverMsgToString(msg) << std::endl;
}

void ComSerialServer::log(std::string loggedMsg) const {
    std::cout << "# " << loggedMsg << std::endl;
}

std::optional<ClientResponse> ComSerialServer::try_parse_buffer(){

    datetime_t d = {};
    // TODO fix this (wtf)
    std::string command(8,0);

    sscanf(this->recv_buffer, "%7s(%hd-%hhd-%hhdT%hhd:%hhd:%hhd)",
                            &d.year, &d.month, &d.day, &d.hour, &d.min, &d.sec);

    ClientResponseType type = SET_TME;
    if(command == "SET_ALM"){
        type = SET_ALM;
    }
    return ClientResponse{.type=type,.time=d};
}

std::optional<ClientResponse> ComSerialServer::try_recv(){
    auto t1 = make_timeout_time_ms(100);

    int read_chars = stdio_get_until(this->recv_buffer, RECV_BUFFER_LEN, t1);

    return try_parse_buffer();
}
