#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "tcpEntity.h"

class TcpClient : public TcpEntity {
  int connectionState;
public:
  TcpClient ();

  bool connect (const char* serverIpAddress, const int serverPort);
  bool receive (char* inputText, int& textLength);
  bool send (char* outputText);
  ~TcpClient ();
};

#endif
