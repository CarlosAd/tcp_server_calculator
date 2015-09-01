#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "tcpEntity.h"

class TcpServer : public TcpEntity {
  struct sockaddr_in address_;
  int bindingState_;
  int newSocketFileDescriptor_;
public:
  TcpServer ();
  bool bind (int portNumber);
  bool listenAndAccept ();

  bool receive (char* inputText, int& textLength);
  bool send (char* outputText);

  ~TcpServer ();
};

#endif
