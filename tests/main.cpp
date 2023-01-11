#include "rtc.h"

#include <iostream>

int main(int argc, char **argv)
{
	CreateWsClient("ws://localhost:9002");
	do
	{
	} while (ws_status::WS_OPEN != GetWsStatus());

	WsSendMsg("Hello");

	return 0;
}
