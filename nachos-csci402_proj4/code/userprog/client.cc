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

void Client::Encode(int argCount, int *args, char *data)
{
    char *buffer = new char[MaxMailSize];
    ASSERT(argCount>0);
    ASSERT(argCount*FIELD_WIDTH<MaxMailSize);
    buffer[0] = argCount;
    int val;
    for (int i=0; i<argCount; i++) {
        val = args[i];
        int remainder = 0;
        for (int j=1; j<=FIELD_WIDTH; j++) {
            remainder = val%10;
            buffer[i*FIELD_WIDTH+j] = remainder;
            val /= 10;
        }
    }
    bcopy(buffer, data, MaxMailSize);
    delete buffer;
}

void Client::Decode(int *argCount, int **args, char *data)
{
    char *buffer = new char[MaxMailSize];
    bcopy(data, buffer, MaxMailSize);
    *argCount = (int)buffer[0];
    ASSERT((*argCount)>=0);
    
    if (*args != NULL) 
        delete *args;
    *args = new int[*argCount];
    
    for (int i=0; i<(*argCount); i++) {
        int val = 0;
        int pow = 1;
        for (int j=1; j<=FIELD_WIDTH; j++) {
            val = val+(int)buffer[i*FIELD_WIDTH+j]*pow;
            pow *= 10;
        }
        (*args)[i] = val;
    }
    delete buffer;
}

bool Client::Connect(NetworkAddress addr, int box)
{
    PacketHeader outPktHdr, inPktHdr;
    MailHeader outMailHdr, inMailHdr;
    char *out = new char[MaxMailSize];
    char *in = new char[MaxMailSize];
    int argCount;
    int *args;

    outPktHdr.to = addr;
    outPktHdr.from = clientaddr;
    outMailHdr.to = box;
    outMailHdr.from = clientbox;
    outMailHdr.length = MaxMailSize;
    
    argCount = 1;
    args = new int[argCount];
    args[0] = NETWORK_SERVER_CONNECT_REQUEST;
    
    Encode(argCount, args, out);
    delete args;
    client->Send(outPktHdr, outMailHdr, out);
    delete out;
    
    client->Receive(clientbox, &inPktHdr, &inMailHdr, in);
    ASSERT(inPktHdr.to == clientaddr);
    ASSERT(inMailHdr.to == clientbox);
    ASSERT(inPktHdr.from == addr);
    ASSERT(inPktHdr.to == clientaddr);
    
    Decode(&argCount, &args, in);
    ASSERT(argCount>0);
    int instr = (int)args[0];
    
    if (instr != NETWORK_SERVER_CONNECT_REPLY) {
        delete in;
        return false;
    }
    
    delete in;
    seraddr = addr;
    serbox = box;
    connect = true;
    
    return true;
}

void Client::DisConnect(NetworkAddress addr)
{
    seraddr = -1;
    serbox = -1;
    connect = false;
}

void Client::Send(int argCount, int *args)
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
    Encode(argCount, args, out);
    
    client->Send(outPktHdr, outMailHdr, out);
    
    delete out;
}

void Client::Receive(int *argCount, int **args)
{
    if (!connect) {
        printf("Should connect to server first\n");
        return;
    }
    
    PacketHeader inPktHdr;
    MailHeader inMailHdr;
    char *in = new char[MaxMailSize];
    
    client->Receive(clientbox, &inPktHdr, &inMailHdr, in);
    ASSERT(inPktHdr.to == clientaddr);
    ASSERT(inMailHdr.to == clientbox);
    ASSERT(inPktHdr.from == seraddr);
    ASSERT(inMailHdr.from == serbox);
    Decode(argCount, args, in);
    delete in;
}

void Client::Print()
{
    cout<<"Client NetAddress: "<<clientaddr<<endl;
    cout<<"Client Receving Box: "<<clientbox<<endl;
    cout<<"Client Connect to Server: "<<seraddr<<endl;
}




