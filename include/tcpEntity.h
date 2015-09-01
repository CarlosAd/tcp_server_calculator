#ifndef TCP_ENTITY_H
#define TCP_ENTITY_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class TcpEntity {
protected:
  int socketFileDescriptor_;
  struct sockaddr_in otherSocketAddress_;
  
public:
  TcpEntity ();
  
  bool send (int socketFileDescriptor, char* outputText);
  bool receive (int socketFileDescriptor, char* inputText, int& textLength);
};

#endif
