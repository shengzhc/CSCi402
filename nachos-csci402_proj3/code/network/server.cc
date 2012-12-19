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

// Send out a msg from the default server mail box
void Server::Reply(Message *msg)
{
    server->Send(msg->pktHdr, msg->mailHdr, msg->data);
    delete msg;
}

// Print out the info of the current server
void Server::Print()
{
    cout<<"Server NetAddress: "<<seraddr<<endl;
    cout<<"Server Receiving Box: "<<serbox<<endl;
}

// Reply to a Connect Request
void Server::ConnectReply(PacketHeader outPktHdr, MailHeader outMailHdr)
{
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_CONNECT_REPLY;
    out[1] = 1;
    
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    //cout<<"Connect Reply"<<endl;
    
    return;
}

// Reply to a CreateLock Request
void Server::CreateLockReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *data)
{
    int length = (int)data[1];
    char *name = new char[length+1];
    for (int i=0; i<length; i++) {
        name[i] = data[i+2];
    }
    name[length] = '\0';
    
    int lockid = slt->SearchName(name);
    
    if (lockid == -1) {
        lockid = slt->AssignLock(name);
    }
    else {
        delete name;
    }
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_CREATELOCK_REPLY;
    out[1] = lockid & 0x000000FF;
    out[2] = lockid & 0x0000FF00;
    
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    cout<<"CreateLock Reply: ["<<lockid<<"]"<<endl;
    return;
    
}

// Reply to a DestroyLock Request
void Server::DestroyLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr)
{
    
    if (lockid<0 || lockid>=SL_MAX) {
        cout<<"error lockid."<<endl;
        return ;
    }

    int rec = slt->Destroy(lockid);
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_DESTROYLOCK_REPLY;
    out[1] = rec;
    
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    delete msg;
    cout<<"DestroyLock Reply: ["<<lockid<<"]"<<endl;
    return;
}

// Reply to a AcquireLock Request
void Server::AcquireLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr)
{
    
    if (lockid<0||lockid>=SL_MAX) {
        cout<<"error lockid."<<endl;
        return ;
    }
    
    int rec = slt->Acquire(lockid, outPktHdr.to*100+outMailHdr.to);
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_ACQUIRELOCK_REPLY;
    out[1] = rec;
    
    Message *msg;
    if (rec == -1) {
        out[1] = 1;
        msg = new Message(outPktHdr, outMailHdr, out);
        slt->Pend(lockid, msg);
        return;
    }
    msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    delete msg;
    cout<<"AcquireLock Reply: ["<<lockid<<"]"<<endl;
    return;

}

// Reply to a ReleaseLock Request
void Server::ReleaseLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr)
{    
    if (lockid<0||lockid>=MAX_OSLOCK) {
        cout<<"error lockid."<<endl;
        return ;
    }

    int rec = slt->Release(lockid, outPktHdr.to*100+outMailHdr.to);
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_RELEASELOCK_REPLY;
    out[1] = rec;
    
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    delete msg;
    
    if(rec == 2) {
        msg = slt->GetMsg(lockid);
        this->Reply(msg);
        slt->SetLockOwner(lockid, msg->pktHdr.to*100+msg->mailHdr.to);
        delete msg;
    }
        
    cout<<"ReleaseLock Reply: ["<<lockid<<"]"<<endl;
    return ;
}

// Reply to a CreateCondition Request
void Server::CreateConditionReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *in)
{
    int length = (int)in[1];
    char *name = new char[length+1];
    for (int i=0; i<length; i++) {
        name[i] = in[2+i];
    }
    name[length] = '\0';
    int cvid = sct->SearchName(name);
    if (cvid == -1) {
        cvid = sct->AssignCondition(name);
    }
    else {
        delete name;
    }
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_CREATECONDITION_REPLY;
    out[1] = cvid & 0x000000FF;
    out[2] = cvid & 0x0000FF00;
    
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    delete msg;
    cout<<"CreateCondition Reply: ["<<cvid<<"]"<<endl;
    return;
    
}

// Reply to a DestroyCondition Request
void Server::DestroyConditionReply(int cvid, PacketHeader outPktHdr, MailHeader outMailHdr)
{
    if (cvid<0||cvid>=SC_MAX) {
        cout<<"error cvid"<<endl;
        return;
    }
    
    int rec = sct->Destroy(cvid);
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_DESTROYCONDITION_REPLY;
    out[1] = rec;
    
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    delete msg;
    cout<<"DestroyCondition Reply: ["<<cvid<<"]"<<endl;
    return;
}

// Reply to a WaitCondition Request
void Server::WaitConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr)
{
    if (cvid<0||cvid>SC_MAX) {
        cout<<"error cvid."<<endl;
        return;
    }
    if (lockid<0||lockid>SC_MAX) {
        cout<<"error lockid."<<endl;
        return;
    }
    
    int rec = sct->Wait(cvid, lockid);
    
    Message *msg;
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_WAITCONDITION_REPLY;
    out[1] = rec;
    
    if (rec == 1) {
        msg = new Message(outPktHdr, outMailHdr, out);
        sct->Pend(cvid, msg);
        int rep = slt->Release(lockid, outPktHdr.to*100+outMailHdr.to);
        if (rep == 2) {
            this->Reply(slt->GetMsg(lockid));
        }
        cout<<"WaitCondition Reply: ["<<cvid<<", "<<lockid<<"]"<<endl;
        return;
    }
    return;
}


// Reply to a SignalCondition Request
void Server::SignalConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr)
{
    if (cvid<0||cvid>=SC_MAX) {
        cout<<"error cvid."<<endl;
        return;
    }
    if (lockid<0||lockid>SL_MAX) {
        cout<<"error lockid."<<endl;
        return;
    }
    
    int rec = sct->Signal(cvid, lockid);
    if (rec == 2) {
        Message *pmsg = sct->GetMsg(cvid);
        slt->Pend(lockid, pmsg);
    }
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_SIGNALCONDITION_REPLY;
    out[1] = rec;
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    delete msg;
    cout<<"SignalCondition Reply: ["<<cvid<<", "<<lockid<<"]"<<endl;
    return;
}

// Reply to a BroadcastCondition Request
void Server::BroadcastConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr)
{
    if (cvid<0||cvid>SC_MAX) {
        cout<<"error cvid."<<endl;
        return;
    }
    if (lockid<0||lockid>SL_MAX) {
        cout<<"error lockid."<<endl;
        return;
    }
    int rec = -1;
    while (true) {
        rec = sct->Signal(cvid, lockid);
        if (rec == 2) {
            Message *pmsg = sct->GetMsg(cvid);
            slt->Pend(lockid, pmsg);
            //this->Reply(sct->GetMsg(cvid));
        }
        if (rec == 1) {
            break;
        }
    }
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_BROADCASTCONDITION_REPLY;
    out[1] = rec;
    
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    delete msg;
    cout<<"BroadcastCondition Reply: ["<<cvid<<", "<<lockid<<"]"<<endl;
    return;
}

// Reply to a CreateMV Request
void Server::CreateMVReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *in)
{
    int length = in[1];
    char *name = new char[length+1];
    for (int i=0; i<length; i++) {
        name[i] = in[2+i];
    }
    name[length] = '\0';
    int mvid = mvt->SearchName(name);
    if (mvid == -1) {
        int size = in[2+length]+256*in[2+length+1];
        mvid = mvt->AssignMV(name, size);
    }
    else {
        delete name;
    }
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_CREATEMV_REPLY;
    out[1] = mvid & 0x000000FF;
    out[2] = mvid & 0x0000FF00;
    
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    delete out;
    cout<<"CreateMV Reply: ["<<mvid<<"]"<<endl;
    return;
}

// Reply to a SetMV Request
void Server::SetMVReply(int mvid, int index, int value, PacketHeader outPktHdr, MailHeader outMailHdr)
{
    int rec = mvt->SetMV(mvid, index, value);
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_SETMV_REPLY;
    out[1] = rec;
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    cout<<"SetMV Reply: ["<<mvid<<", "<<index<<", "<<value<<"]"<<endl;
    delete msg;
    return;
}

// Reply to a GetMV Request
void Server::GetMVReply(int mvid, int index, PacketHeader outPktHdr, MailHeader outMailHdr)
{
    int rec = mvt->GetMV(mvid, index);
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_GETMV_REPLY;
    out[1] = rec & 0x000000FF;
    out[2] = rec & 0x0000FF00;
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    cout<<"GetMV Reply: ["<<mvid<<", "<<index<<", "<<rec<<"]"<<endl;
    delete msg;
    return;
}

// Reply to a DestroyMV Request
void Server::DestroyMVReply(int mvid, PacketHeader outPktHdr, MailHeader outMailHdr)
{
    int rec = mvt->DestroyMV(mvid);
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_DESTROYMV_REPLY;
    out[1] = rec;
    
    Message *msg = new Message(outPktHdr, outMailHdr, out);
    this->Reply(msg);
    cout<<"DestroyMV Reply: ["<<mvid<<"]"<<endl;
    delete msg;
    return;
}

// Essential function that runs the server
void Server::Run()
{
    cout<<"Server is Running."<<endl;
    PacketHeader outPktHdr, inPktHdr;
    MailHeader inMailHdr, outMailHdr;
    char *in = new char[MaxMailSize];
    char *out;
    Message *msg;
    while (true) {
        server->Receive(serbox, &inPktHdr, &inMailHdr, in);
        ASSERT(inPktHdr.to == seraddr);
        ASSERT(inMailHdr.to == serbox);
        outPktHdr.to = inPktHdr.from;
        outPktHdr.from = seraddr;
        outMailHdr.to = inMailHdr.from;
        outMailHdr.from = serbox;
        outMailHdr.length = MaxMailSize;
        switch ((int)in[0]) {
            case NETWORK_SERVER_CONNECT_REQUEST:
                //cout<<"Accept Connect Request"<<endl;
                ConnectReply(outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_CREATELOCK_REQUEST:
                cout<<"Accept CreateLock Request"<<endl;
                CreateLockReply(outPktHdr, outMailHdr, in);
                break;
            case NETWORK_SERVER_DESTROYLOCK_REQUEST:
                cout<<"Accept DestroyLock Request"<<endl;
                DestroyLockReply(in[1]+in[2]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_ACQUIRELOCK_REQUEST:
                cout<<"Accept AcquireLock Request"<<endl;
                AcquireLockReply(in[1]+in[2]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_RELEASELOCK_REQUEST:
                cout<<"Accept ReleaseLock Request"<<endl;
                ReleaseLockReply(in[1]+in[2]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_CREATECONDITION_REQUEST:
                cout<<"Accept CreateCondition Request"<<endl;
                CreateConditionReply(outPktHdr, outMailHdr, in);
                break;
            case NETWORK_SERVER_DESTROYCONDITION_REQUEST:
                cout<<"Accept DestroyCondition Request"<<endl;
                DestroyConditionReply(in[1]+in[2]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_WAITCONDITION_REQUEST:
                cout<<"Accept WaitCondition Request"<<endl;
                WaitConditionReply(in[1]+in[2]*256, in[3]+in[4]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_SIGNALCONDITION_REQUEST:
                cout<<"Accept SignalCondition Request"<<endl;
                SignalConditionReply(in[1]+in[2]*256, in[3]+in[4]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_BROADCASTCONDITION_REQUEST:
                cout<<"Accept BroadCast Condition Request"<<endl;
                BroadcastConditionReply(in[1]+in[2]*256, in[3]+in[4]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_CREATEMV_REQUEST:
                cout<<"Accept CreateMV Request"<<endl;
                CreateMVReply(outPktHdr, outMailHdr, in);
                break;
            case NETWORK_SERVER_SETMV_REQUEST:
                cout<<"Accept SetMV Request"<<endl;
                SetMVReply(in[1]+in[2]*256, in[3]+in[4]*256, in[5]+in[6]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_GETMV_REQUEST:
                cout<<"Accept GetMV Request"<<endl;
                GetMVReply(in[1]+in[2]*256, in[3]+in[4]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_DESTROYMV_REQUEST:
                cout<<"Accept DestroyMV Request"<<endl;
                DestroyMVReply(in[1]+in[2]*256, outPktHdr, outMailHdr);
                break;
            case NETWORK_SERVER_CLOSE_REQUEST:
                cout<<"Accept Close Server Request"<<endl;
                cout<<"Reply Close ACK"<<endl;
                return;
                
            default:
                cout<<"error reach ";
                break;
        }
    }
}

