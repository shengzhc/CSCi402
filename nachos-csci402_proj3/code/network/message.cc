#include <iostream>
#include "message.h"

using namespace std;
// Construct the Message
Message::Message(PacketHeader phdr, MailHeader mhdr, char *buffer)
{
    pktHdr.to = phdr.to;
    pktHdr.from = phdr.from;
    pktHdr.length = phdr.length;
    
    mailHdr.to = mhdr.to;
    mailHdr.from = mhdr.from;
    mailHdr.length = mhdr.length;
    
    data = new char[mailHdr.length];
    strcpy(data, buffer);
}

Message::~Message()
{
    if (data != NULL) {
        delete data;
    }
}

