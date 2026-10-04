#include "types/types.h"
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
