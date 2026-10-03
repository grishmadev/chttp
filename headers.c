#include "types.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * Used to build headers
 */
char *build_headers(const Header *headers, size_t count) {
  if (!headers && count > 0)
    return NULL;

  size_t total_size = 3;
  for (int i = 0; i < count; i++) {
    total_size += strlen(headers[i].key) + strlen(headers[i].value) + 4;
  }

  char *result = malloc(total_size);
  if (!result)
    return NULL;

  char *ptr = result;
  size_t remaining = total_size;

  for (size_t i = 0; i < count; i++) {
    int written = snprintf(ptr, remaining, "%s: %s\r\n", headers[i].key,
                           headers[i].value);
    ptr += written;
    remaining -= (size_t)written;
  }

  snprintf(ptr, remaining, "\r\n");

  return result;
}

int handle_client(void *arg) {
  char serMsg[255] = "Hello World";
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
        {"Content-Type", "text/html"},
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
