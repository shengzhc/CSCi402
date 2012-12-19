#ifndef Nachos_client_h
#define Nachos_client_h

#include "post.h"

// Define a class representing the behavior of a Client
class Client {
    
public:
    Client(NetworkAddress addr, double reliability, int nBoxes, int box);
    ~Client();
    bool Connect(NetworkAddress addr, int box);
    void DisConnect();
    void Send(char *);
    void Receive(char **);
    void Print();
private:    
    PostOffice *client; // a underlying component controlling receive & send Msg
    NetworkAddress clientaddr; // define the client machine id
    NetworkAddress seraddr; // define the server machine id
    int clientbox; // define the default server mail box id
    int serbox; // define the connected server mail box id
    bool connect; // define a flag indicating the connect status
};


#endif
