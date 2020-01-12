#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

sig_atomic_t volatile g_running = 1;
int client_socket;
int server_socket;
FILE *index_file;

void sig_handler(int signum)
{
  printf("\n User provided signal handler for Ctrl+C \n");

  if (signum == SIGINT) {
    g_running = 0;
  }

  shutdown(client_socket, 2);
  shutdown(server_socket, 2);
  fclose(index_file);

  exit(1);
}

int main(int argc, char *argv[]) {
  int port = atoi(argv[1]);


  index_file = fopen("index.html", "r");

  printf("Server Starting\n");

  if(index_file == NULL) {
    perror("Error opeing file: ");
    exit(1);
  }

  char response_data[1024];
  size_t newLen = fread(response_data, sizeof(char), 1024, index_file);

  if (ferror(index_file) != 0) {
    fputs("Error reading file", stderr);
  } else {
    response_data[newLen++] = '\0';
  }

  char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";

  strcat(http_header, response_data);

  // Socket stuff

  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = INADDR_ANY;

 int status = bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

 if(status != 0) {
   printf("Error on bind, status: %d\n", status);
   exit(1);
 }

  listen(server_socket, 5);

  printf("Listening\n");


  char request[4096];
  char method[4];

  signal(SIGINT, &sig_handler);

  while(g_running) {
    printf("g_running %d\n", g_running);
    printf("Waiting for Client\n");
    client_socket = accept(server_socket, NULL, NULL);

    recv(client_socket, &request, 4096, 0);

    memcpy(method, &request[0], 3);
    method[4] = '\0';

    printf("Request method:\n%s\n", method);
    printf("Full Request:\n%s\n", request);
    send(client_socket, http_header, sizeof(http_header), 0);
    shutdown(client_socket, 2);
  }

  shutdown(server_socket, 2);
  fclose(index_file);
  return 0;
}
