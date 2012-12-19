#ifndef Nachos_client_h
#define Nachos_client_h

#include "post.h"

// Define a class representing the behavior of a Client
class Client {
    
public:
    Client(NetworkAddress addr, double reliability, int nBoxes);
    ~Client();
    void Send(char *, NetworkAddress sid, int cmid, int length);
    void Receive(char **, int cmid);
private:    
    PostOffice *client; // a underlying component controlling receive & send Msg
    NetworkAddress clientaddr; // define the client machine id
};


#endif
