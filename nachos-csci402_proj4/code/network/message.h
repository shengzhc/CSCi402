#ifndef Nachos_message_h
#define Nachos_message_h

#include "post.h"
#include "list.h"
#include <time.h>

// Define a send/receive entity
class Message {
    
public:
    Message(PacketHeader phdr, MailHeader mhdr, char *buffer);
    Message(PacketHeader phdr, MailHeader mhdr, char *buffer, int64_t t);
    void SetTime(int64_t t) {timestamp = t;}
    ~Message() {}
public:
    PacketHeader pktHdr;
    MailHeader mailHdr;
    int64_t timestamp;
    char data[MaxMailSize];
};


#endif
