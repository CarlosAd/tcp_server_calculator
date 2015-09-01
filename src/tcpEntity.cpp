#include "tcpEntity.h"
#include <string.h>
#include <stdio.h>

#define BUFFER_MAX_LENGTH 512

TcpEntity::TcpEntity () {
  socketFileDescriptor_ = socket (AF_INET, SOCK_STREAM, 0);
#ifdef TEST_BUILD
  perror("socket");
#endif
  if (socketFileDescriptor_ < 0){
    perror("socket");
  }
}

bool TcpEntity::receive (int socketFileDescriptor, char* inputText, int& textLength) {
  textLength = recvfrom (socketFileDescriptor, inputText, BUFFER_MAX_LENGTH, 0, NULL, NULL);
#ifdef TEST_BUILD
  perror ("recvfrom()");
#endif
  if (textLength <= 0) {
    return false;
  }
  inputText[textLength] = '\0';
  return true;
}

bool TcpEntity::send (int socketFileDescriptor, char* outputText) {
  int sendingState = sendto (socketFileDescriptor, outputText, strlen(outputText), 0, (struct sockaddr*) &otherSocketAddress_, sizeof(otherSocketAddress_));
#ifdef TEST_BUILD
  perror ("sendto()");
#endif
  if (sendingState <=0) {
    perror ("sendto()");
    return false;
  }
  return true;
}
