#include "tcpServer.h"
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

#define LISTEN_QUEUE_MAX_SIZE 5

TcpServer::TcpServer () : TcpEntity(), newSocketFileDescriptor_ (-1), bindingState_ (-1) {
}

bool TcpServer::bind (int portNumber) {
  memset ((char*) &address_, 0, sizeof(address_));

  address_.sin_family = AF_INET;
  address_.sin_addr.s_addr = INADDR_ANY;
  address_.sin_port = htons (portNumber);

  bindingState_ = ::bind (socketFileDescriptor_, (struct sockaddr*) &address_, sizeof(address_));
#ifdef TEST_BUILD
  perror ("bind");
#endif

  if (bindingState_ < 0) {
    perror ("bind");
    return false;
  }
  return true;
}

bool TcpServer::listenAndAccept() {
  listen (socketFileDescriptor_, LISTEN_QUEUE_MAX_SIZE);
#ifdef TEST_BUILD
  perror ("listen()");
#endif

  socklen_t otherSocketAddressLength_ = sizeof(otherSocketAddress_);
  newSocketFileDescriptor_ = accept (socketFileDescriptor_, (struct sockaddr*) &otherSocketAddress_, &otherSocketAddressLength_);
#ifdef TEST_BUILD
  perror("accept()");
#endif

  if (newSocketFileDescriptor_ < 0){
    perror("accept()");
    return false;
  }

  return true;
}

bool TcpServer::receive (char* inputText, int& textLength) {
  return TcpEntity::receive (newSocketFileDescriptor_, inputText, textLength);
}

bool TcpServer::send (char* outputText) {
  return TcpEntity::send (newSocketFileDescriptor_, outputText);
}

TcpServer::~TcpServer () {
  close (socketFileDescriptor_);
}
