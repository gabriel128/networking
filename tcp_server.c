#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {

  char message[512] = "Wooooot";

  int nsocket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8001);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int status = bind(nsocket, (struct sockaddr *) &server_address, sizeof(server_address));

  if(status != 0) {
    printf("Error, status: %d\n", status);
    exit(1);
  }

  listen(nsocket, 5);

  while(1) {
    int client_socket = accept(nsocket, NULL, NULL);

    send(client_socket, message, sizeof(message), 0);

  }
  shutdown(nsocket, 2);
  return 0;
}
