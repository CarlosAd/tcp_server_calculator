#include "tcpServer.h"
#include "tcpClient.h"
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

typedef vector< vector < int > > matrix;
typedef vector< int > row;
typedef vector< int > column;

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
  int numberOfColumns, numberOfRows;

  sprintf (sendingBuffer, "Number of Rows:");
  server.send (sendingBuffer);
  server.receive (receivingBuffer, textLength);
  numberOfRows = atoi (receivingBuffer);

  sprintf (sendingBuffer, "Number of Columns:");
  server.send (sendingBuffer);
  server.receive (receivingBuffer, textLength);
  numberOfColumns = atoi (receivingBuffer);

  for (int i = 0; i < numberOfRows; i++) {
    inMatrix.push_back(row());
    for (int j = 0; j < numberOfColumns; j++){
      sprintf (sendingBuffer, "Element (%d, %d):", i+1, j+1);
      server.send (sendingBuffer);
      server.receive (receivingBuffer, textLength);
      inMatrix[i].push_back(atoi(receivingBuffer));
    }
  }
}

matrix addition (matrix& firstMatrix, matrix& secondMatrix) {
  matrix res;
  int numberOfRows = firstMatrix.size();
  int numberOfColumns = firstMatrix[0].size();

  for (int i = 0; i < numberOfRows; i++) {
    res.push_back(row());
    for (int j = 0; j < numberOfColumns; j++) {
      res[i].push_back(firstMatrix[i][j] + secondMatrix[i][j]);
    }
  }
  return res;
}

matrix subtraction (matrix& firstMatrix, matrix& secondMatrix) {
  matrix res;
  int numberOfRows = firstMatrix.size();
  int numberOfColumns = firstMatrix[0].size();

  for (int i = 0; i < numberOfRows; i++) {
    res.push_back (row());
    for (int j = 0; j < numberOfColumns; j++) {
      res[i].push_back (firstMatrix[i][j] - secondMatrix[i][j]);
    }
  }
  return res;
}

matrix multiplation (matrix& firstMatrix, matrix& secondMatrix) {
  int resNumberOfRows = firstMatrix.size();
  int resNumberOfCols = secondMatrix[0].size();
  matrix res;

  for (int i = 0; i < resNumberOfRows; i++) {
    res.push_back (row (resNumberOfCols, 0));
    for (int j = 0; j < resNumberOfCols; j++) {
      for (int k = 0; k < secondMatrix.size(); k++) {
        res[i][j] += firstMatrix[i][k]*secondMatrix[k][j];
      }
    }
  }
  return res;
}

matrix transpose (matrix& m) {
  matrix res;
  for (int j = 0; j < m[0].size(); j++) {
    res.push_back(row());
    for (int i = 0; i < m.size(); i++) {
      res[j].push_back(m[i][j]);
    }
  }
  return res;
}

void sendMenu (TcpServer& server) {
  sprintf (sendingBuffer, "Operations\n1 - Addition\n2 - Subtraction\n3 - Multiplication\n4 - Transpose\nChoose one operation:");
  server.send (sendingBuffer);
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

  sendMenu(server);
  server.receive (receivingBuffer, textLength);

  int operationOption = atoi (receivingBuffer);

  if ((operationOption != 1) && (operationOption != 2) && (operationOption != 3) && (operationOption != 4)){
    sprintf (sendingBuffer, "Invalid Option.\n");
    server.send (sendingBuffer);
    return 0;
  }

  matrix result;

  if (operationOption != 4){

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
  }
  else {
    sprintf (sendingBuffer, "\nMatrix:\n"); 
    server.send (sendingBuffer);

    readMatrixFromClient (server, firstMatrix);

    sprintf (sendingBuffer, "\nMatrix:\n"); 
    server.send (sendingBuffer);

    printMatrixToBuffer (firstMatrix);          
    server.send (sendingBuffer);

    result = transpose (firstMatrix);
  }

  sprintf (sendingBuffer, "\nResulting Matrix:\n");
  server.send (sendingBuffer);

  printMatrixToBuffer (result);
  server.send (sendingBuffer);


  delete[] sendingBuffer;
}
#endif

#ifdef CLIENT
int main(int argc, char** argv){
  if (argc < 2) {
    printf ("ERROR: Chamada incorreta.\nEscreva '<nome do programa> <endereço IP> <porta>'.\n");
    return 0;
  }

  receivingBuffer = sendingBuffer = new char[512];
  matrix clientMatrix;

  TcpClient client;
  client.connect (argv[1], atoi(argv[2]));

  while (client.receive (receivingBuffer, textLength)) {
    cout << receivingBuffer;
    if (receivingBuffer[textLength-1]  == ':'){
      cin >> sendingBuffer;
      client.send (sendingBuffer);
    }
  }

  delete[] sendingBuffer;
}
#endif
