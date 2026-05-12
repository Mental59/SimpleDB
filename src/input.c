#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <input.h>

static size_t k_initial_buf_cap = 256;

InputBuffer* new_input_buffer(void)
{
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));

  if (input_buffer)
  {
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
  }

  return input_buffer;
}

void free_input_buffer(InputBuffer* input_buffer)
{
  if (input_buffer)
  {
    free(input_buffer->buffer);
    free(input_buffer);
  }
}

long read_input_line(FILE* stream, char** out, size_t* capacity)
{
  if (!stream || !out || !capacity)
  {
    return -1;
  }

  if (*capacity == 0)
  {
    *out = malloc(k_initial_buf_cap);
    if (!(*out))
    {
      return -1;
    }

    *capacity = k_initial_buf_cap;
  }

  (*out)[0] = '\0';

  size_t len = 0;
  while (1)
  {
    size_t avail = *capacity - len;
    if (avail < 2)
    {
      size_t new_cap = *capacity * 2;
      char* tmp = realloc(*out, new_cap);
      if (!tmp)
      {
        return -1;
      }

      *out = tmp;
      *capacity = new_cap;
      avail = new_cap - len;
    }

    if (!fgets(*out + len, (int)avail, stream))
    {
      break; // EOF after partial line
    }

    len += strlen(*out + len);

    if (len > 0 && (*out)[len - 1] == '\n')
    {
      break; // full line read
    }
  }

  return (long)len;
}

void read_input(InputBuffer* input_buffer)
{
  long bytes_read = read_input_line(stdin, &(input_buffer->buffer),
                                    &(input_buffer->buffer_length));

  if (bytes_read <= 0)
  {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[input_buffer->input_length] = '\0';
}
