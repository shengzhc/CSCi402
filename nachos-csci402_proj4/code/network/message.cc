#include <iostream>
#include "message.h"

using namespace std;
// Construct the Message
extern "C" {
	int bcopy(char *, char *, int);
};
Message::Message(PacketHeader phdr, MailHeader mhdr, char *buffer)
{
    pktHdr.to = phdr.to;
    pktHdr.from = phdr.from;
    pktHdr.length = phdr.length;
    
    mailHdr.to = mhdr.to;
    mailHdr.from = mhdr.from;
    mailHdr.length = mhdr.length;
        
    bcopy(buffer, data, mailHdr.length);
}


Message::Message(PacketHeader phdr, MailHeader mhdr, char *buffer, int64_t t)
{
    pktHdr.to = phdr.to;
    pktHdr.from = phdr.from;
    pktHdr.length = phdr.length;
    
    mailHdr.to = mhdr.to;
    mailHdr.from = mhdr.from;
    mailHdr.length = mhdr.length;

    bcopy(buffer, data, mailHdr.length);

    timestamp = t;
}

