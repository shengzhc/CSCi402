#include <iostream>
#include "server.h"
#include "system.h"
#include "protocol.h"

extern "C" {
	int bcopy(char *, char *, int);
};

using namespace std;

Server::Server(NetworkAddress addr, double reliability, int nBoxes, int box)
{
    ASSERT(nBoxes>=box);
    server = new PostOffice(addr, reliability, nBoxes);
    seraddr = addr;
    serbox = box;
}

Server::~Server()
{
    if (server != NULL)
    delete server;
}

void Server::Reply(PacketHeader outPktHdr, MailHeader outMailHdr, int argCount, int *args)
{
    char *out = new char[MaxMailSize];
    Encode(argCount, args, out);
    server->Send(outPktHdr, outMailHdr, out);
    delete out;
}

void Server::Encode(int argCount, int *args, char *data)
{
    char *buffer = new char[MaxMailSize];
    ASSERT(argCount>0);
    ASSERT(argCount*FIELD_WIDTH<MaxMailSize);
    buffer[0] = (int)argCount;
    for (int i=0; i<argCount; i++) {
        int val = args[i];
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

void Server::Decode(int *argCount, int **args, char *data)
{
    char *buffer = new char[MaxMailSize];
    bcopy(data, buffer, MaxMailSize);
    *argCount = (int)buffer[0];
    ASSERT((*argCount)>=0);
    
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

void Server::Print()
{
    cout<<"Server NetAddress: "<<seraddr<<endl;
    cout<<"Server Receiving Box: "<<serbox<<endl;
}

int Server::CreateLock()
{
    int lockid;
    osLockTableLock->Acquire();
    for (lockid=0;lockid<MAX_OSLOCK;lockid++) {
        if (osLockTable[lockid].isDeleted) {
            osLockTable[lockid].isDeleted = false;
            break;
        }
    }
    if (lockid >= MAX_OSLOCK) {
        osLockTableLock -> Release();
        return MAX_OSLOCK;
    }
    if (osLockTable[lockid].lock != NULL) {
        delete osLockTable[lockid].lock;
    }
    osLockTable[lockid].lock = new Lock("");
    osLockTable[lockid].addrSpace = currentThread->space;
    osLockTableLock->Release();
    return lockid;
}

int Server::DestroyLock(int lockid)
{
    if (lockid<0 || lockid>=MAX_OSLOCK) {
        printf("error lockid.\n");
        return 0;
    }
    osLockTableLock -> Acquire();
    if (currentThread->space != osLockTable[lockid].addrSpace) {
        printf("Different process.\n");
        osLockTableLock->Release();
        return 0;
    }
    if (osLockTable[lockid].isDeleted) {
        printf("Lock has been deleted");
    }
    else {
        osLockTable[lockid].isToBeDeleted = true;;
    }
    osLockTableLock -> Release();
    return 1;
}

void Server::Run()
{
    cout<<"Server is Running."<<endl;
    PacketHeader outPktHdr, inPktHdr;
    MailHeader inMailHdr, outMailHdr;
    char *in = new char[MaxMailSize];
    while (true) {
        server->Receive(serbox, &inPktHdr, &inMailHdr, in);
        ASSERT(inPktHdr.to == seraddr);
        ASSERT(inMailHdr.to == serbox);
        outPktHdr.to = inPktHdr.from;
        outMailHdr.to = inMailHdr.from;
        outMailHdr.from = serbox;
        outMailHdr.length = MaxMailSize;
        int instr;
        int argCount;
        int *inargs = NULL;
        int *outargs;
        Decode(&argCount, &inargs, in);
        instr = inargs[0];
        switch (instr) {
            case NETWORK_SERVER_CONNECT_REQUEST:
                argCount = 1;
                outargs = new int[argCount];
                outargs[0] = NETWORK_SERVER_CONNECT_REPLY;
                this->Reply(outPktHdr, outMailHdr, argCount, outargs);
                cout<<"Reply Connect ACK"<<endl;
                delete outargs;
                break;
            case NETWORK_SERVER_CLOSE_REQUEST:
                argCount = 1;
                outargs = new int[argCount];
                outargs[0] = NETWORK_SERVER_CLOSE_REPLY;
                this->Reply(outPktHdr, outMailHdr, argCount, outargs);
                cout<<"Reply Close ACK"<<endl;
                delete outargs;
                return;
            case NETWORK_SERVER_CREATELOCK_REQUEST:
                argCount = 2;
                outargs = new int[argCount];
                outargs[0] = NETWORK_SERVER_CREATELOCK_REPLY;
                outargs[1] = this->CreateLock();
                this->Reply(outPktHdr, outMailHdr, argCount, outargs);
                cout<<"Reply CreateLock ACK"<<endl;
                delete outargs;
                break;
            case NETWORK_SERVER_DESTROYLOCK_REQUEST:
                argCount = 2;
                outargs = new int[argCount];
                outargs[0] = NETWORK_SERVER_DESTROYLOCK_REPLY;
                outargs[1] = this->DestroyLock(inargs[1]);
                this->Reply(outPktHdr, outMailHdr, argCount, outargs);
                cout<<"Reply DestroyLock ACK"<<endl;
                delete outargs;
                break;
            default:
                cout<<"error reach ";
                cout<<instr<<endl;
            break;
        }
        delete inargs;
    }
}

