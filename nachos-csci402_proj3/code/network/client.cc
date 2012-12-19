#include <iostream>
#include "client.h"
#include "protocol.h"

extern "C" {
	int bcopy(char *, char *, int);
};

using namespace std;

Client::Client(NetworkAddress addr, double reliability, int nBoxes, int box)
{
    ASSERT(box<nBoxes);
    client = new PostOffice(addr, reliability, nBoxes);
    clientaddr = addr;
    clientbox = box;
    seraddr = -1;
    serbox = -1;
    connect = false;
}

Client::~Client()
{
    if (client != NULL) 
    delete client;
}

// Connect to a server
bool Client::Connect(NetworkAddress addr, int box)
{
    PacketHeader outPktHdr, inPktHdr;
    MailHeader outMailHdr, inMailHdr;
    char *out = new char[MaxMailSize];
    char *in = new char[MaxMailSize];
    
    outPktHdr.to = addr;
    outPktHdr.from = clientaddr;
    outMailHdr.to = box;
    outMailHdr.from = clientbox;
    outMailHdr.length = MaxMailSize;
    
    out[0] = NETWORK_SERVER_CONNECT_REQUEST;
    client->Send(outPktHdr, outMailHdr, out);
    delete out;
    
    client->Receive(clientbox, &inPktHdr, &inMailHdr, in);
    ASSERT(inPktHdr.to == clientaddr);
    ASSERT(inMailHdr.to == clientbox);
    ASSERT(inPktHdr.from == addr);
    ASSERT(inPktHdr.to == clientaddr);
    
    if ((int)in[0] != NETWORK_SERVER_CONNECT_REPLY) {
        cout<<"Connect error"<<endl;
        delete in;
        return false;
    }
    delete in;
    seraddr = addr;
    serbox = box;
    connect = true;
    return true;
}

// Disconnect the current server
void Client::DisConnect()
{
    seraddr = -1;
    serbox = -1;
    connect = false;
}

// Client sends the data to server
void Client::Send(char *data)
{
    if (!connect) {
        printf("Should connect to server first\n");
        return;
    }
    
    PacketHeader outPktHdr;
    MailHeader outMailHdr;
    
    outPktHdr.to = seraddr;
    outPktHdr.from = clientaddr;
    outMailHdr.to = serbox;
    outMailHdr.from = clientbox;
    outMailHdr.length = MaxMailSize;
    
    char *out = new char[MaxMailSize];
    bcopy(data, out, MaxMailSize);
    
    client->Send(outPktHdr, outMailHdr, out);
    delete out;
}

// Client tries to receive the data from server
void Client::Receive(char **data)
{
    if (!connect) {
        printf("Should connect to server first\n");
        return;
    }
    
    PacketHeader inPktHdr;
    MailHeader inMailHdr;
    char *in = new char[MaxMailSize];
    *data = new char[MaxMailSize];
    
    client->Receive(clientbox, &inPktHdr, &inMailHdr, in);
    ASSERT(inPktHdr.to == clientaddr);
    ASSERT(inMailHdr.to == clientbox);
    ASSERT(inPktHdr.from == seraddr);
    ASSERT(inMailHdr.from == serbox);
    
    bcopy(in, *data, MaxMailSize);
    delete in;
}

// Print out the info of Client 
void Client::Print()
{
    cout<<"Client NetAddress: "<<clientaddr<<endl;
    cout<<"Client Receving Box: "<<clientbox<<endl;
    cout<<"Client Connect to Server: "<<seraddr<<endl;
}




