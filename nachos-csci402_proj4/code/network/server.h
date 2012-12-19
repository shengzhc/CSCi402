#ifndef Nachos_server_h
#define Nachos_server_h

#include "post.h"
#include "message.h"
#include "list.h"
#include "synch.h"
// Define a class representing the server
#define phdrSize (sizeof(PacketHeader))
#define mhdrSize (sizeof(MailHeader))
#define timeSize (sizeof(int64_t))

class Server {
    
public:
    Server(NetworkAddress addr, double reliability, int nBoxes, int volum);
    ~Server();
    void Run();
    void Reply(Message *msg);
    void Print();
    void CreateLockReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *data, bool reply);
    void DestroyLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void AcquireLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void ReleaseLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void CreateConditionReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *in, bool reply);
    void DestroyConditionReply(int cvid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void WaitConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void SignalConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void BroadcastConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void CreateMVReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *in, bool reply);
    void SetMVReply(int mvid, int index, int value, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void GetMVReply(int mvid, int index, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void DestroyMVReply(int mvid, PacketHeader outPktHdr, MailHeader outMailHdr, bool reply);
    void Propagate();
    void ReceiptThread();
    bool IsUpdated();
    void ProcessdMsg();

private:
    PostOffice *server; // Send and Receive Msg component
    NetworkAddress seraddr; // Server ID
    int64_t *ltr;
    List *mqueue;
    Lock *mqlock;
    Condition *mqcv;
    Message *exmsg;
    bool updated[5];
    int64_t min_t;
    bool busy;
    int cap;
};

#endif
