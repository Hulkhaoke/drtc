#include "peer_connection.h"

PeerConnection::PeerConnection()
{
}

PeerConnection::~PeerConnection()
{
}

int PeerConnection::Init(WsClient *ws_client)
{
    if(nullptr == ws_client)
    {
        printf("Invalid ws_client ptr\n");
        return -1;
    }

    ws_client_ = ws_client;

    ice_agent_.CreateIceAgent(
        [](juice_agent_t *agent, juice_state_t state, void *user_ptr)
        {

        },
        [](juice_agent_t *agent, const char *sdp, void *user_ptr)
        {

        },
        [](juice_agent_t *agent, void *user_ptr)
        {

        },
        [](juice_agent_t *agent, const char *data, size_t size, void *user_ptr)
        {

        }
        ,
        this
    );
    return 0;
}

int PeerConnection::GetLocalSdp()
{
    local_sdp_ = ice_agent_.GenerateLocalSdp();
    return 0;
}

int PeerConnection::SetRemoteSdp(const std::string &remote_sdp)
{
    ice_agent_.SetRemoteSdp(remote_sdp.c_str());
    return 0;
}

int PeerConnection::SendOffer()
{
    ws_client_->Send(local_sdp_);
    return 0;
}