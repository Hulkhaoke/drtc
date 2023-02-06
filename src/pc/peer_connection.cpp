#include "peer_connection.h"
#include "log.h"
#include <nlohmann/json.hpp>

#include <map>

using nlohmann::json;

static const std::map<std::string, unsigned int> siganl_types{
    {"connection_id", 1},
    {"offer", 2}};

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
        [](juice_agent_t *agent, juice_state_t state, void *user_ptr)
        {
            LOG_INFO("state_change: {}", state);
        },
        [](juice_agent_t *agent, const char *sdp, void *user_ptr)
        {
            LOG_INFO("candadite: {}", sdp);
        },
        [](juice_agent_t *agent, void *user_ptr)
        {
            LOG_INFO("gather_done");
        },
        [](juice_agent_t *agent, const char *data, size_t size, void *user_ptr)
        {
            LOG_INFO("on_recv");
        },
        this);
    return 0;
}

int PeerConnection::CreateTransport()
{
    LOG_INFO("Create transport");
    
    if(SignalStatus::Connected != signal_status_)
        return -1;

    json message = {
        {"type", "create_transport"}
    };

    Send(message.dump());
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
    json message = {
        {"type", "offer"},
        {"sdp", local_sdp_}
    };
    LOG_INFO("Send local sdp:\n{}", message.dump().c_str());

    Send(message.dump());
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
    auto j = json::parse(msg);

    std::string type = j["type"];
    auto itr = siganl_types.find(type);
    if (itr != siganl_types.end())
    {
        LOG_INFO("msg type :{}", itr->second);
        switch (itr->second)
        {
        case 1:
            connection_id_ = j["connection_id"].get<unsigned int>();
            LOG_INFO("Receive local peer connection_id [{}]", connection_id_);
            signal_status_ = SignalStatus::Connected;
            break;
        
        default:
            break;
        }
    }
}
