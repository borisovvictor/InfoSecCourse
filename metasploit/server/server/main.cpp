
#define DEFAULT_BUFLEN 512

#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <string>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc, char *argv[])
{
  WSADATA wsa;
  SOCKET s, new_socket;
  struct sockaddr_in server, client;
  int c;
  char *message;

  char recvbuf[DEFAULT_BUFLEN];
  int recvbuflen = DEFAULT_BUFLEN;

  printf("\nInitialising Winsock...");
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    printf("Failed. Error Code : %d", WSAGetLastError());
    return 1;
  }

  printf("Initialised.\n");

  //Create a socket
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
    printf("Could not create socket : %d", WSAGetLastError());
  }

  printf("Socket created.\n");

  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(6792);

  //Bind
  if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
  {
    printf("Bind failed with error code : %d", WSAGetLastError());
  }

  puts("Bind done");

  while (true)
  {
    //Listen to incoming connections
    listen(s, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    new_socket = accept(s, (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET)
    {
      printf("accept failed with error code : %d", WSAGetLastError());
    }

    puts("Connection accepted");

    int iResult = recv(new_socket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
      if (std::string(recvbuf).find("is_it_test_tcp_server") != std::string::npos)
      {
        //Reply to client
        message = "yes_it_is";
        send(new_socket, message, strlen(message), 0);
      }
    }

  }
  getchar();

  closesocket(s);
  WSACleanup();

  return 0;
}
