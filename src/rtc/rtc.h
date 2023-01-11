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

int rtc();

int CreateWsClient(const char* uri);

int WsSendMsg(const char* message);

ws_status GetWsStatus();


#endif