#include "headers.c"
#include "parse/parse.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int handle_client(void *arg) {
  Response success = {1, "Hello"};
  char *serMsg = parse_success(&success);
  char content_length[16];
  snprintf(content_length, sizeof(content_length), "%zu", strlen(serMsg));
  int csock = *(int *)arg;
  free(arg);

  while (1) {
    char buf[1024];
    int read_res = read(csock, buf, 1024);
    if (read_res <= 0) {
      // close(csock);
      break;
    }

    Header headers[] = {
        {"Content-Type", "application/json"},
        {"Content-Length", content_length},
        {"Connection", "keep-alive"},
    };

    size_t count = sizeof(headers) / sizeof(headers[0]);
    char *h_str = build_headers(headers, count);
    if (!h_str) {
      close(csock);
      return 1;
    }

    char response[2048];
    snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\n%s%s", h_str,
             serMsg);

    if (send(csock, response, strlen(response), 0) < 0) {
      printf("Could not send data\n");
      return 1;
    };

    free(h_str);
  }
  close(csock);
  return 0;
}
