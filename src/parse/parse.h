#include <stddef.h>
typedef enum {
  JSON_NULL,
  JSON_STRING,
  JSON_NUMBER,
  JSON_BOOL,
} JsonType;

typedef struct JsonValue JsonValue;

typedef struct {
  char *key;
  JsonValue *value;
} JsonMember;

struct JsonValue {
  JsonType type;
  union {
    int boolean;
    double number;
    char *string;
    struct {
      JsonMember *members;
      size_t count;
    } object;
  } value;
};

typedef struct {
  int success;
  char *message;
} Response;
