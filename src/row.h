#ifndef SIMPLEDB_ROW_H
#define SIMPLEDB_ROW_H

#include <stdint.h>

#include <defines.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define USERNAME_MAX_CHARS 31
#define EMAIL_MAX_CHARS 254

typedef struct
{
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;

enum
{
  ID_SIZE = SIZE_OF_ATTRIBUTE(Row, id),
  USERNAME_SIZE = SIZE_OF_ATTRIBUTE(Row, username),
  EMAIL_SIZE = SIZE_OF_ATTRIBUTE(Row, email),
  ID_OFFSET = 0,
  USERNAME_OFFSET = ID_OFFSET + ID_SIZE,
  EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE,
  ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE
};

void serialize_row(Row *source, void *destination);
void deserialize_row(void *source, Row *destination);

#endif
