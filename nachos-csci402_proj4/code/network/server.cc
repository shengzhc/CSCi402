#include <iostream>
#include <sys/time.h>
#include "server.h"
#include "system.h"
#include "protocol.h"

using namespace std;
extern "C" {
	int bcopy(char *, char *, int);
};

int64_t GetTime();
void STDelegate(int arg);

int64_t GetTime() {
 
    int64_t a, b, c, d;
    timeval t;
    gettimeofday(&t, NULL);
    a = t.tv_sec;
    b = a * 1000000;
    c = b+t.tv_usec;
    d = c*10;
    d+=5;
    return d;
}

void STDelegate(int arg) 
{
    Server *s = (Server *)arg;
    s->ReceiptThread();
}

Server::Server(NetworkAddress addr, double reliability, int nBoxes, int volum)
{
    server = new PostOffice(addr, reliability, nBoxes);
    seraddr = addr;
    cap = volum;
    ltr = new int64_t[cap+1];
    for (int i=0; i<=cap; i++) {
        ltr[i] = 0;
        updated[i] = false;
    }
    mqueue = new List();
    mqlock = new Lock("mqlock");
    mqcv = new Condition("mqcv");
    busy = false;
}

Server::~Server()
{
    if (server != NULL)
        delete server;
    delete ltr;
    delete mqueue;
    delete mqlock;
    delete mqcv;
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
}

// Reply to a CreateLock Request
void Server::CreateLockReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *data, bool reply)
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
    
    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_CREATELOCK_REPLY;
        bcopy((char*)&lockid, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        cout<<"CreateLock Reply: ["<<lockid<<"]"<<endl;
        delete msg;
    }
    return;
}

// Reply to a DestroyLock Request
void Server::DestroyLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
{
    
    if (lockid<0 || lockid>=SL_MAX) {
        cout<<"error lockid."<<endl;
        return ;
    }

    int rec = slt->Destroy(lockid);
    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_DESTROYLOCK_REPLY;
        bcopy((char*)&rec, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        cout<<"DestroyLock Reply: ["<<lockid<<"]"<<endl;
        delete msg;
    }
    return;
}

// Reply to a AcquireLock Request
void Server::AcquireLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
{
    
    if (lockid<0||lockid>=SL_MAX) {
        cout<<"error lockid."<<endl;
        return ;
    }
    
    int rec = slt->Acquire(lockid, outPktHdr.to*100+outMailHdr.to);
    
    char *out = new char[MaxMailSize];
    out[0] = NETWORK_SERVER_ACQUIRELOCK_REPLY;
    bcopy((char*)&rec, &out[1], sizeof(int));
    outMailHdr.length = 1+sizeof(int);
    
    Message *msg;
    if (rec == -1) {
        out[1] = 1;
        msg = new Message(outPktHdr, outMailHdr, out);
        slt->Pend(lockid, msg);
        return;
    }
    if (reply) {
        msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        delete msg;
        cout<<"AcquireLock Reply: ["<<lockid<<"]"<<endl;
    }
    return;

}

// Reply to a ReleaseLock Request
void Server::ReleaseLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
{    
    if (lockid<0||lockid>=MAX_OSLOCK) {
        cout<<"error lockid."<<endl;
        return ;
    }

    int rec = slt->Release(lockid, outPktHdr.to*100+outMailHdr.to);
    
    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_RELEASELOCK_REPLY;
        bcopy((char*)&rec, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        cout<<"ReleaseLock Reply: ["<<lockid<<"]"<<endl;
        delete msg;
    }
    if(rec == 2) {
        Message *msg = slt->GetMsg(lockid);
        if (reply) {
            this->Reply(msg);
        }
        slt->SetLockOwner(lockid, msg->pktHdr.to*100+msg->mailHdr.to);
        delete msg;
    }
    return ;
}

// Reply to a CreateCondition Request
void Server::CreateConditionReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *in, bool reply)
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
    if (reply) {
        char *out = new char[3];
        out[0] = NETWORK_SERVER_CREATECONDITION_REPLY;
        bcopy((char*)&cvid, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        delete msg;
        cout<<"CreateCondition Reply: ["<<cvid<<"]"<<endl;
    }
    return;
    
}

// Reply to a DestroyCondition Request
void Server::DestroyConditionReply(int cvid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
{
    if (cvid<0||cvid>=SC_MAX) {
        cout<<"error cvid"<<endl;
        return;
    }
    
    int rec = sct->Destroy(cvid);
    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_DESTROYCONDITION_REPLY;
        bcopy((char*)&rec, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        delete msg;
        cout<<"DestroyCondition Reply: ["<<cvid<<"]"<<endl;
    }
    return;
}

// Reply to a WaitCondition Request
void Server::WaitConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
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
    bcopy((char*)&rec, &out[1], sizeof(int));
    outMailHdr.length = 1+sizeof(int);

    if (rec == 1) {

        msg = new Message(outPktHdr, outMailHdr, out);
        sct->Pend(cvid, msg);

        int rep = slt->Release(lockid, outPktHdr.to*100+outMailHdr.to);
        if (rep == 2) {
            msg = (Message*)slt->GetMsg(lockid);
            if (reply) {
                this->Reply(msg);
                cout<<"WaitCondition Reply: ["<<cvid<<", "<<lockid<<"]"<<endl;
            }
            delete msg;
        }

        return;
    }
    return;
}

// Reply to a SignalCondition Request
void Server::SignalConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
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
    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_SIGNALCONDITION_REPLY;
        bcopy((char*)&rec, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        cout<<"SignalCondition Reply: ["<<cvid<<", "<<lockid<<"]"<<endl;
        delete msg;
    }
    return;
}

// Reply to a BroadcastCondition Request
void Server::BroadcastConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
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
    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_BROADCASTCONDITION_REPLY;
        bcopy((char*)&rec, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        delete msg;
        cout<<"BroadcastCondition Reply: ["<<cvid<<", "<<lockid<<"]"<<endl;
    }
    return;
}

// Reply to a CreateMV Request
void Server::CreateMVReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *in, bool reply)
{
    int length = in[1];
    char *name = new char[length+1];
    for (int i=0; i<length; i++) {
        name[i] = in[2+i];
    }
    name[length] = '\0';
    int mvid = mvt->SearchName(name);
    if (mvid == -1) {
        int size;
        bcopy(&in[2+length], (char*)&size, sizeof(int));
        mvid = mvt->AssignMV(name, size);
    }
    else {
        delete name;
    }
    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_CREATEMV_REPLY;
        bcopy((char*)&mvid, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        delete out;
        cout<<"CreateMV Reply: ["<<mvid<<"]"<<endl;
    }
    return;
}

// Reply to a SetMV Request
void Server::SetMVReply(int mvid, int index, int value, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
{

    int rec = mvt->SetMV(mvid, index, value);
    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_SETMV_REPLY;
        bcopy((char*)&rec, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        cout<<"SetMV Reply: ["<<mvid<<", "<<index<<", "<<rec<<"]"<<endl;
        delete msg;
    }
    return;
}

// Reply to a GetMV Request
void Server::GetMVReply(int mvid, int index, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
{
    int rec = mvt->GetMV(mvid, index);

    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_GETMV_REPLY;
        bcopy((char*)&rec, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        cout<<"GetMV Reply: ["<<mvid<<", "<<index<<", "<<rec<<"]"<<endl;

        delete msg;
    }
    return;
}

// Reply to a DestroyMV Request
void Server::DestroyMVReply(int mvid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply)
{
    int rec = mvt->DestroyMV(mvid);
    if (reply) {
        char *out = new char[MaxMailSize];
        out[0] = NETWORK_SERVER_DESTROYMV_REPLY;
        bcopy((char*)&rec, &out[1], sizeof(int));
        outMailHdr.length = 1+sizeof(int);
        Message *msg = new Message(outPktHdr, outMailHdr, out);
        this->Reply(msg);
        cout<<"DestroyMV Reply: ["<<mvid<<"]"<<endl;
        delete msg;
    }
    return;
}

// Send msg to other servers
void Server::Propagate()
{
    PacketHeader outPktHdr;
    MailHeader outMailHdr;
    PacketHeader inPktHdr;
    MailHeader inMailHdr;
    ASSERT(exmsg != NULL);
    char *data = new char[MaxMailSize];
    data[0] = NETWORK_SERVER_MESSAGESHARE_REQUEST;
    data[1] = exmsg->pktHdr.to;
    data[2] = exmsg->pktHdr.from;
    data[3] = exmsg->mailHdr.to;
    data[4] = exmsg->mailHdr.from;
    data[5] = exmsg->mailHdr.length;
    bcopy((char*)(&exmsg->timestamp), &data[6], timeSize);   
    bcopy(exmsg->data, &data[6+timeSize], exmsg->mailHdr.length);
    for (int i=1; i<=cap; i++) {
        if (i == seraddr) {
            continue;
        }
        else {
            outPktHdr.to = i;
            outPktHdr.from = seraddr;
            outMailHdr.to = 1;
            outMailHdr.from = 1;
            outMailHdr.length = 6+timeSize+exmsg->mailHdr.length;
            server->Send(outPktHdr, outMailHdr, data);
        }
    }
    exmsg = NULL;
    delete [] data;
}

bool Server::IsUpdated()
{
    for (int i=1; i<=cap; i++) {
        if (!updated[i]) {
            return false;
        }
    }
    return true;
}

// Thread used for communication between servers
void Server::ReceiptThread() 
{
    PacketHeader inPktHdr, outPktHdr, tempPktHdr;
    MailHeader inMailHdr, outMailHdr, tempMailHdr;
    int64_t timestamp;
    char *in = new char[MaxMailSize];
    int sid;
    while (true) {
        server->Receive(1, &inPktHdr, &inMailHdr, in);
        switch (in[0]) {
            case NETWORK_SERVER_MESSAGESHARE_REQUEST:
                char *data = new char[MaxMailSize];
                tempPktHdr.to = in[1];
                tempPktHdr.from = in[2];
                tempMailHdr.to = in[3];
                tempMailHdr.from = in[4];
                tempMailHdr.length = in[5];
                bcopy(&in[6], (char*)&timestamp, timeSize);
                bcopy(&in[6+timeSize], data, tempMailHdr.length);
                Message *msg = new Message(tempPktHdr, tempMailHdr, data, timestamp);
                delete data;
                mqlock->Acquire();
                mqueue->SortedInsert(msg, timestamp);
                sid = tempPktHdr.to;
                updated[sid] = true;
                ltr[sid] = timestamp;
                outPktHdr.from = seraddr;
                outMailHdr.from = 1;
                outMailHdr.to = 1;
                timestamp = GetTime();
                ltr[seraddr] = timestamp;
                updated[seraddr] = true;
                data = new char[MaxMailSize];
                data[0] = NETWORK_SERVER_MESSAGESHARE_REPLY;
                data[1] = seraddr;
                bcopy((char*)(&timestamp), &data[2], timeSize);
                outMailHdr.length = 2+timeSize;
                for (int i=1; i<=cap; i++) {
                    if (i != seraddr) {
                        outPktHdr.to = i;
                        server->Send(outPktHdr, outMailHdr, data);
                    }
                }
                if (IsUpdated()) {
                    if (busy) {
                        mqcv->Signal(mqlock);   
                    }
                    else {
                        ProcessdMsg();
                    }
                }

                delete data;
                data = NULL;
                mqlock->Release();
                break;
            case NETWORK_SERVER_MESSAGESHARE_REPLY:
                sid = in[1];
                bcopy(&in[2], (char*)&timestamp, timeSize);
                mqlock->Acquire();
                ltr[sid] = timestamp;
                updated[sid] = true;
                if (IsUpdated()) {
                    if (busy) {
                        mqcv->Signal(mqlock);   
                    }
                    else {
                        ProcessdMsg();
                    }

                }
                mqlock->Release();
                break;
            default:
                break;
        }
        currentThread->Yield();
    }
    delete [] in;
}

void Server::ProcessdMsg()
{
    Message *msg;
    PacketHeader outPktHdr;
    MailHeader outMailHdr;
    char *in = new char[MaxMailSize];
    min_t = ltr[1];
    int lockid, cvid, mvid, t;
    for (int i=1; i<=cap; i++) {
        if (min_t>ltr[i]) {
            min_t = ltr[i];
        }
        updated[i] = false;
    }
    while (true) {
        msg = (Message*)(mqueue->SortedRemove(NULL));
        if (msg == NULL) {
            break;
        }
        if (msg->timestamp>min_t) {
            break;
        }
        outPktHdr.to = msg->pktHdr.from;
        outPktHdr.from = seraddr;
        outMailHdr.to = msg->mailHdr.from;
        outMailHdr.from = 0;
        outMailHdr.length = MaxMailSize;
        bool flag = (seraddr==msg->pktHdr.to);
        bcopy(msg->data, in, MaxMailSize);
        switch ((int)in[0]) {
            case NETWORK_SERVER_CREATELOCK_REQUEST:
                cout<<"Accept CreateLock Request"<<endl;
                CreateLockReply(outPktHdr, outMailHdr, in, flag);
                break;
            case NETWORK_SERVER_DESTROYLOCK_REQUEST:
                cout<<"Accept DestroyLock Request"<<endl;
                bcopy(&in[1], (char*)&lockid, sizeof(int));
                DestroyLockReply(lockid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_ACQUIRELOCK_REQUEST:
                cout<<"Accept AcquireLock Request "<<endl;
                bcopy(&in[1], (char*)&lockid, sizeof(int));
                AcquireLockReply(lockid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_RELEASELOCK_REQUEST:
                cout<<"Accept ReleaseLock Request "<<endl;
                bcopy(&in[1], (char*)&lockid, sizeof(int));
                ReleaseLockReply(lockid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_CREATECONDITION_REQUEST:
                cout<<"Accept CreateCondition Request"<<endl;
                CreateConditionReply(outPktHdr, outMailHdr, in, flag);
                break;
            case NETWORK_SERVER_DESTROYCONDITION_REQUEST:
                cout<<"Accept DestroyCondition Request"<<endl;
                bcopy(&in[1], (char*)&cvid, sizeof(int));
                DestroyConditionReply(cvid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_WAITCONDITION_REQUEST:
                cout<<"Accept WaitCondition Request"<<endl;
                bcopy(&in[1], (char*)&cvid, sizeof(int));
                bcopy(&in[1+sizeof(int)], (char*)&lockid, sizeof(int));
                WaitConditionReply(cvid, lockid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_SIGNALCONDITION_REQUEST:
                cout<<"Accept SignalCondition Request"<<endl;
                bcopy(&in[1], (char*)&cvid, sizeof(int));
                bcopy(&in[1+sizeof(int)], (char*)&lockid, sizeof(int));
                SignalConditionReply(cvid, lockid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_BROADCASTCONDITION_REQUEST:
                cout<<"Accept BroadCast Condition Request"<<endl;
                bcopy(&in[1], (char*)&cvid, sizeof(int));
                bcopy(&in[1+sizeof(int)], (char*)&lockid, sizeof(int));
                BroadcastConditionReply(cvid, lockid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_CREATEMV_REQUEST:
                cout<<"Accept CreateMV Request"<<endl;
                CreateMVReply(outPktHdr, outMailHdr, in, flag);
                break;
            case NETWORK_SERVER_SETMV_REQUEST:
                cout<<"Accept SetMV Request "<<endl;
                bcopy(&in[1], (char*)&cvid, sizeof(int));
                bcopy(&in[1+sizeof(int)], (char*)&lockid, sizeof(int));
                bcopy(&in[1+2*sizeof(int)], (char*)&mvid, sizeof(int));
                SetMVReply(cvid, lockid, mvid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_GETMV_REQUEST:
                cout<<"Accept GetMV Request "<<endl;
                bcopy(&in[1], (char*)&cvid, sizeof(int));
                bcopy(&in[1+sizeof(int)], (char*)&lockid, sizeof(int));
                GetMVReply(cvid, lockid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_DESTROYMV_REQUEST:
                cout<<"Accept DestroyMV Request"<<endl;
                bcopy(&in[1], (char*)&cvid, sizeof(int));
                DestroyMVReply(cvid, outPktHdr, outMailHdr, flag);
                break;
            case NETWORK_SERVER_CLOSE_REQUEST:
                cout<<"Accept Close Server Request"<<endl;
                cout<<"Reply Close ACK"<<endl;
                return;                
            default:
                cout<<"error reach"<<(int)in[0]<<endl;
                break;
        }
        //cout<<msg->timestamp<<endl;
        delete msg;
    }
    if (msg != NULL) {
        mqueue->SortedInsert((void*)msg, (int64_t)msg->timestamp);
    }
    delete [] in;
}

// Essential function that runs the server
void Server::Run()
{
    cout<<"Server["<<seraddr<<"] is Running."<<endl;
    PacketHeader inPktHdr;
    MailHeader inMailHdr;
    char *in = new char[MaxMailSize];
    Message *msg = NULL;
    Thread *th = new Thread("csz");
    th->Fork(STDelegate, (int)this);
    while (true) {
        server->Receive(0, &inPktHdr, &inMailHdr, in);
        ASSERT(inPktHdr.to == seraddr);
        ASSERT(inMailHdr.to == 0);
        int64_t ts = GetTime();
        msg = new Message(inPktHdr, inMailHdr, in, ts);
        mqlock->Acquire();
        ltr[seraddr] = ts;
        updated[seraddr] = true;
        mqueue->SortedInsert((void*)msg, (int64_t)ts);
        exmsg = msg;
        Propagate();
        busy = true;
        if (cap != 1) {
            mqcv->Wait(mqlock);
        }
        busy = false;
        ProcessdMsg();
        mqlock->Release();
    }
    delete [] in;
}

