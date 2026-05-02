#ifndef SIMPLEDB_INPUT_H
#define SIMPLEDB_INPUT_H

#include <stddef.h>
#include <stdio.h>

typedef struct
{
  char *buffer;
  size_t buffer_length;
  size_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer(void);
void free_input_buffer(InputBuffer *input_buffer);
long read_line(FILE *stream, char **out, size_t *capacity);
void read_input(InputBuffer *input_buffer);

#endif /* SIMPLEDB_INPUT_H */
