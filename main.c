#include "headers.c"
#include <asm-generic/socket.h>
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <threads.h>
#include <unistd.h>

#define PORT 8080
int main(int argc, char const *argv[]) {

  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    printf("Socket creation failed\n");
    return 1;
  }

  int opt = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct timeval tv = {.tv_sec = 5, .tv_usec = 0};

  char serMsg[255] = "Message from the server to the "
                     "client \'Hello Client\' ";

  struct sockaddr_in serv;
  serv.sin_family = AF_INET;
  serv.sin_port = htons(PORT);
  serv.sin_addr.s_addr = INADDR_ANY;

  if (bind(s, (struct sockaddr *)&serv, sizeof(serv))) {
    printf("Could not bind to device\b");
    close(s);
    return 1;
  };

  if (listen(s, SOMAXCONN)) {
    printf("Listening Error\n");
    close(s);
    return 1;
  };
  printf("Listening on port: %d..\n", PORT);

  while (1) {
    int csock = accept(s, NULL, NULL);
    if (csock < 0) {
      printf("Could not accept connections\n");
      continue;
    }
    if (setsockopt(csock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))) {
      perror("setsockopt SO_RCVTIMEO error occured.\n");
    }

    int *pclient = malloc(sizeof(int));
    if (!pclient) {
      close(csock);
      continue;
    }

    *pclient = csock;
    pthread_t thread;
    if (thrd_create(&thread, handle_client, pclient) == 0) {
      thrd_detach(thread);
    } else {
      free(pclient);
      close(csock);
    };
  }
  close(s);
  return 0;
}
