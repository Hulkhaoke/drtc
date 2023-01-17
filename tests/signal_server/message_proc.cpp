#include "message_proc.h"
#include <iostream>
#include <nlohmann/json.hpp>

using nlohmann::json;

MessageProc::MessageProc()
{

}

MessageProc::~MessageProc()
{

}

void MessageProc::Process(const std::string &msg)
{
    std::cout << "Receive message: [ " << msg << " ]" << std::endl;
    auto j = json::parse(msg);

    std::string type = j["type"];
    std::string sdp = j["sdp"];

    std::cout << "Message type: " << type << std::endl;
    std::cout << "Message body: " << sdp << std::endl;
}