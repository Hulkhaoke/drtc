#ifndef _PEER_CONNECTION_H_ 
#define _PEER_CONNECTION_H_ 

#include "ice_agent.h"
#include "ws_client.h"

#include <iostream>

class PeerConnection
{
public:
    PeerConnection();

    ~PeerConnection();

    int Init(WsClient *ws_client);

    int GetLocalSdp();

    int SetRemoteSdp(const std::string &remote_sdp);

    int SendOffer();

private:
    WsClient *ws_client_ = nullptr;
    IceAgent ice_agent_;
    std::string local_sdp_;
    std::string remote_sdp_;
    std::string local_candidates_;
    std::string remote_candidates_;
};

#endif