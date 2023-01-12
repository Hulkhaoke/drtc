#include "peer_connection.h"

#include <nlohmann/json.hpp>

using nlohmann::json;

PeerConnection::PeerConnection()
{
}

PeerConnection::~PeerConnection()
{
}

int PeerConnection::Init(std::string const &uri)
{
    Connect(uri);

    ice_agent_.CreateIceAgent(
        [](juice_agent_t *agent, juice_state_t state, void *user_ptr) {

        },
        [](juice_agent_t *agent, const char *sdp, void *user_ptr) {

        },
        [](juice_agent_t *agent, void *user_ptr) {

        },
        [](juice_agent_t *agent, const char *data, size_t size, void *user_ptr) {

        },
        this);
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

int PeerConnection::SendLocalSdp()
{
    json message = {{"type", "offer"},
                    {"sdp", local_sdp_}};
    Send(message);
    return 0;
}

int PeerConnection::AddRemoteCandidate(const std::string &remote_candidate)
{
    ice_agent_.AddRemoteCandidates(remote_candidate.c_str());
    return 0;
}

int PeerConnection::CreateOffer()
{
    GetLocalSdp();
    SendLocalSdp();
    ice_agent_.GatherCandidates();

    return 0;
}

void PeerConnection::OnReceiveMessage(const std::string &msg)
{
    std::cout << "Receive msg: " << msg << std::endl;
}
