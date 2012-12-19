#ifndef Nachos_message_h
#define Nachos_message_h

#include "post.h"
#include "list.h"

// Define a send/receive entity
class Message {
    
public:
    Message(PacketHeader phdr, MailHeader mhdr, char *buffer);
    ~Message();
public:
    PacketHeader pktHdr;
    MailHeader mailHdr;
    char *data;
};


#endif
