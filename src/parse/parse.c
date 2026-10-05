#include "parse.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *parse_success(const Response *res) {
  char *success = res->success ? "true" : "false";
  char *message = res->message;
  int len = snprintf(NULL, 0, "{\"success\": %s, \"message\": \"%s\"}", success,
                     message);
  // +1 for null terminator
  size_t total_size = (size_t)len + 1;
  char *msg = malloc(total_size);
  if (!msg)
    return NULL;
  snprintf(msg, total_size, "{\"success\": %s, \"message\": \"%s\"}", success,
           message);
  return msg;
}
