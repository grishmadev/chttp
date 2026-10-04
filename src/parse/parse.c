#include "parse.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *parse_success(const Response *res) {
  char *success = res->success ? "true" : "false";
  char *message = res->message;
  size_t total_size = strlen("{\"success\":") + strlen(success) +
                      strlen(",\"message\":") + strlen(message) + strlen("\"}");
  char *msg = malloc(total_size);
  char *ptr = msg;
  snprintf(ptr, total_size, "{\n\"success\": %s,\"message\":\"%s\"", success,
           message);
  return msg;
}
