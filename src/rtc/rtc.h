#ifndef _RTC_H_
#define _RTC_H_

enum ws_status
{
    WS_CONNECTING = 0,
    WS_OPEN,
    WS_FAILED,
    WS_CLOSED,
    WS_UNKNOWN
};

#ifdef  __cplusplus
extern "C" {
#endif

int rtc();

int CreatePeerConnection(const char* uri);

int CreateWsClient(const char* uri);

int WsSendMsg(const char* message);

ws_status GetWsStatus();

#ifdef  __cplusplus
}
#endif


#endif