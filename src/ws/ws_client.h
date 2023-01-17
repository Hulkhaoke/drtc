#ifndef _WS_CLIENT_H_
#define _WS_CLIENT_H_

#include "ws_core.h"

class WsClient: public WsCore
{
public:
    WsClient() {}
    ~WsClient() {}

    void OnReceiveMessage(const std::string &msg);
};

#endif