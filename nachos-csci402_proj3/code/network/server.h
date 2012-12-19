#ifndef Nachos_server_h
#define Nachos_server_h

#include "post.h"
#include "message.h"

// Define a class representing the server

class Server {
    
public:
    Server(NetworkAddress addr, double reliability, int nBoxes, int box);
    ~Server();
    void Run();
    void Reply(Message *msg);
    void Print();
    void ConnectReply(PacketHeader outPktHdr, MailHeader outMailHdr);
    void CreateLockReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *data);
    void DestroyLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr);
    void AcquireLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr);
    void ReleaseLockReply(int lockid, PacketHeader outPktHdr, MailHeader outMailHdr);
    void CreateConditionReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *in);
    void DestroyConditionReply(int cvid, PacketHeader outPktHdr, MailHeader outMailHdr);
    void WaitConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr);
    void SignalConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr);
    void BroadcastConditionReply(int cvid, int lockid, PacketHeader outPktHdr, MailHeader outMailHdr);
    void CreateMVReply(PacketHeader outPktHdr, MailHeader outMailHdr, char *in);
    void SetMVReply(int mvid, int index, int value, PacketHeader outPktHdr, MailHeader outMailHdr);
    void GetMVReply(int mvid, int index, PacketHeader outPktHdr, MailHeader outMailHdr);
    void DestroyMVReply(int mvid, PacketHeader outPktHdr, MailHeader outMailHdr);

private:
    PostOffice *server; // Send and Receive Msg component
    NetworkAddress seraddr; // Server ID
    int serbox; // Server default mail box
    
};

#endif
