#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
  char *address = argv[1];
  char *port = argv[2];

  int nsocket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(port));
  inet_aton(address, (struct in_addr *) &server_address.sin_addr.s_addr);

  int status = connect(nsocket, (struct sockaddr *) &server_address, sizeof(server_address));

  if(status != 0) {
    printf("Error on connection, status: %d\n", status);
    exit(1);
  }

  char request[64] = "GET / HTTP/1.1\r\n\r\n";
  char response[4096];

  send(nsocket, request, sizeof(request), 0);

  recv(nsocket, &response, 4096, 0);


  printf("The response is: %s\n", response);


  shutdown(nsocket, 2);
  return 0;
}
