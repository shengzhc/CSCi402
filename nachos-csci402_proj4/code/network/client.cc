#include <iostream>
#include "client.h"
#include "protocol.h"

extern "C" {
	int bcopy(char *, char *, int);
};

using namespace std;

Client::Client(NetworkAddress addr, double reliability, int nBoxes)
{
    client = new PostOffice(addr, reliability, nBoxes);
    clientaddr = addr;
}

Client::~Client()
{
    if (client != NULL) 
    delete client;
}


// Client sends the data to server
void Client::Send(char *data, NetworkAddress sid, int cmbid, int length)
{

    PacketHeader outPktHdr;
    MailHeader outMailHdr;
    
    usleep(500);
    outPktHdr.to = sid;
    outPktHdr.from = clientaddr;
    outMailHdr.to = 0;
    outMailHdr.from = cmbid;
    outMailHdr.length = length;
    char *out = new char[length];
    bcopy(data, out, length);
    client->Send(outPktHdr, outMailHdr, out);
    delete out;
}

// Client tries to receive the data from server
void Client::Receive(char **data, int cmbid)
{
    PacketHeader inPktHdr;
    MailHeader inMailHdr;
    char *in = new char[MaxMailSize];
    *data = new char[MaxMailSize];
    
    client->Receive(cmbid, &inPktHdr, &inMailHdr, in);
    ASSERT(inPktHdr.to == clientaddr);
    ASSERT(inMailHdr.to == cmbid);

    bcopy(in, *data, MaxMailSize);
    delete in;
}




