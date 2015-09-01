#include "tcpServer.h"
#include "tcpClient.h"
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

typedef vector< vector < int > > matrix;
char *receivingBuffer, *sendingBuffer;
int textLength;

void printMatrix (matrix& m) {
  for (int i=0; i<m.size(); i++){
    for (int j=0; j<m[i].size(); j++){
      cout << m[i][j] << " ";
    }
    cout << endl;
  }
}

void printMatrixToBuffer (matrix& m) {
  int pos = 0;
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      pos += sprintf (sendingBuffer + pos, "%d ", m[i][j]);
    }
    pos += sprintf (sendingBuffer + pos, "\n");
  }
}

void readMatrixFromClient (TcpServer& server, matrix& inMatrix) {
  inMatrix.clear();
  strcpy (sendingBuffer, "Number of Rows:");
  server.send (sendingBuffer);
  server.receive (receivingBuffer, textLength);
  inMatrix.resize(atoi (receivingBuffer));

  strcpy (sendingBuffer, "Number of Columns:");
  server.send (sendingBuffer);
  server.receive (receivingBuffer, textLength);
  int numberOfCols = atoi (receivingBuffer);
  for (int i = 0; i < inMatrix.size(); i++) {
    inMatrix[i].resize (numberOfCols);

    for (int j = 0; j < inMatrix[i].size(); j++){
      sprintf (sendingBuffer, "Element (%d, %d):", i+1, j+1);
      server.send (sendingBuffer);
      server.receive (receivingBuffer, textLength);
      inMatrix[i][j] = atoi(receivingBuffer);
    }
  }
}

matrix addition (matrix& firstMatrix, matrix& secondMatrix) {
  matrix res (firstMatrix.size(), vector< int > (firstMatrix[0].size()));

  for (int i = 0; i < firstMatrix.size(); i++) {
    for (int j = 0; j < firstMatrix[i].size(); j++) {
      res[i][j] = firstMatrix[i][j] + secondMatrix[i][j];
    }
  }
  return res;
}

matrix subtraction (matrix& firstMatrix, matrix& secondMatrix) {
  matrix res (firstMatrix.size(), vector< int > (firstMatrix[0].size()));

  for (int i = 0; i < firstMatrix.size(); i++) {
    for (int j = 0; j < firstMatrix[i].size(); j++) {
      res[i][j] = firstMatrix[i][j] - secondMatrix[i][j];
    }
  }
  return res;
}

matrix multiplation (matrix& firstMatrix, matrix& secondMatrix) {
  int resNumberOfRows = firstMatrix.size();
  int resNumberOfCols = secondMatrix[0].size();
  matrix res (resNumberOfRows, vector< int > (resNumberOfCols));

  for (int i = 0; i < resNumberOfRows; i++) {
    for (int j = 0; j < resNumberOfCols; j++) {
      for (int k = 0; k < secondMatrix.size(); k++) {
        res[i][j] += firstMatrix[i][k]*secondMatrix[k][j];
      }
    }
  }
  return res;
}

#ifdef SERVER
int main(int argc, char** argv) {
  receivingBuffer = sendingBuffer = new char[512];
  matrix firstMatrix, secondMatrix;
  TcpServer server;

  if (!server.bind (atoi(argv[1]))){
    return 0;
  }

  if (!server.listenAndAccept()){
    return 0;
  }

  sprintf (sendingBuffer, "Operations\n1 - Addition\n2 - Subtraction\n3 - Multiplication\nChoose one operation:");
  server.send (sendingBuffer);
  server.receive (receivingBuffer, textLength);

  int operationOption = atoi(receivingBuffer);

  if ((operationOption != 1) && (operationOption != 2) && (operationOption != 3)){
    sprintf (sendingBuffer, "Invalid Option.\n");
    server.send (sendingBuffer);
    return 0;
  }

  sprintf (sendingBuffer, "\nFirst Matrix:\n"); 
  server.send (sendingBuffer);

  readMatrixFromClient (server, firstMatrix);

  sprintf (sendingBuffer, "\nFirst Matrix:\n"); 
  server.send (sendingBuffer);

  printMatrixToBuffer (firstMatrix);          
  server.send (sendingBuffer);

  sprintf (sendingBuffer, "\nSecond Matrix:\n");
  server.send (sendingBuffer);
  
  readMatrixFromClient (server, secondMatrix);

  sprintf (sendingBuffer, "\nSecond Matrix:\n");
  server.send (sendingBuffer);

  printMatrixToBuffer (secondMatrix);
  server.send (sendingBuffer);

  matrix result;

  switch (operationOption){
    case 1:
      if ((firstMatrix.size() != secondMatrix.size()) || (firstMatrix[0].size() != secondMatrix[0].size())){
        sprintf (sendingBuffer, "Dimensions don't match. Operation canceled.\n");
        server.send (sendingBuffer);
        return 0;
      }
      result = addition (firstMatrix, secondMatrix);
      break;
    case 2:
      if ((firstMatrix.size() != secondMatrix.size()) || (firstMatrix[0].size() != secondMatrix[0].size())){
        sprintf (sendingBuffer, "Dimensions don't match. Operation canceled.\n");
        server.send (sendingBuffer);
        return 0;
      }
      result = subtraction (firstMatrix, secondMatrix);
      break;
    case 3:
      if (firstMatrix[0].size() != secondMatrix.size()){
        sprintf (sendingBuffer, "Dimensions don't match. Operation canceled.\n");
        server.send (sendingBuffer);
        return 0;
      }
      result = multiplation (firstMatrix, secondMatrix);
      break;
  }

  sprintf (sendingBuffer, "Resulting Matrix:\n");
  server.send (sendingBuffer);

  printMatrixToBuffer (result);
  server.send (sendingBuffer);

  delete[] sendingBuffer;
}
#endif

#ifdef CLIENT
int main(int argc, char** argv){
  receivingBuffer = sendingBuffer = new char[512];
  matrix clientMatrix;

  TcpClient client;
  client.connect ("192.168.1.103", atoi(argv[1]));

  for (int i=0; i<10; i++){
    cin >> sendingBuffer;
    client.send (sendingBuffer);
  }

  /*

  while (client.receive (receivingBuffer, textLength)) {
    cout << receivingBuffer;
    if (receivingBuffer[textLength-1]  == ':'){
      cin >> sendingBuffer;
      client.send (sendingBuffer);
    }
  }
  */

  delete[] sendingBuffer;
}
#endif
