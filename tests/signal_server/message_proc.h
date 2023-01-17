#ifndef _MESSAGE_PROC_H_
#define _MESSAGE_PROC_H_

#include <string>

class MessageProc
{
public:
    MessageProc();
    ~MessageProc();

    void Process(const std::string &msg);
};

#endif