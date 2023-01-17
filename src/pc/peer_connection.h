#ifndef _PEER_CONNECTION_H_ 
#define _PEER_CONNECTION_H_ 

#include "ice_agent.h"
#include "ws_client.h"

#include <iostream>

class PeerConnection:public WsCore
{
public:
    PeerConnection();

    ~PeerConnection();

    int Init(std::string const & uri);

    int GetLocalSdp();

    int SetRemoteSdp(const std::string &remote_sdp);

    int SendLocalSdp();

    int AddRemoteCandidate(const std::string &remote_candidate);

    int CreateOffer();

    void OnReceiveMessage(const std::string &msg);

private:
    IceAgent ice_agent_;
    std::string local_sdp_;
    std::string remote_sdp_;
    std::string local_candidates_;
    std::string remote_candidates_;
};

#endif