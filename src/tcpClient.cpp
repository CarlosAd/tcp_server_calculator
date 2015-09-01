#include "tcpClient.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

TcpClient::TcpClient () : TcpEntity(), connectionState(-1){
}

bool TcpClient::connect (const char* serverIpAddress, const int serverPort) {
  memset ((char*) &otherSocketAddress_, 0, sizeof (otherSocketAddress_));

  otherSocketAddress_.sin_family = AF_INET;
  otherSocketAddress_.sin_addr.s_addr = inet_addr (serverIpAddress);
  otherSocketAddress_.sin_port = htons (serverPort);

  connectionState = ::connect (socketFileDescriptor_, (struct sockaddr*)&otherSocketAddress_, sizeof(otherSocketAddress_));
#ifdef TEST_BUILD
  perror("connect()");
#endif
  if (connectionState < 0){
    perror("connect()");
    return false;
  }
  return true;
}

bool TcpClient::receive (char* inputText, int& textLength) {
  return TcpEntity::receive (socketFileDescriptor_, inputText, textLength);
}

bool TcpClient::send (char* outputText) {
  return TcpEntity::send (socketFileDescriptor_, outputText);
}

TcpClient::~TcpClient () {
  close (connectionState);
  close (socketFileDescriptor_);
}
