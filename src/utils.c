#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include <utils.h>
#include <defines.h>

char *next_token(char **cursor, const char *delims)
{
  if (cursor == NULL || *cursor == NULL || delims == NULL)
  {
    return NULL;
  }

  // Skip leading delimiters
  char *start = *cursor + strspn(*cursor, delims);
  if (*start == '\0')
  {
    *cursor = NULL;
    return NULL;
  }

  // Find end of token
  char *end = start + strcspn(start, delims);
  if (*end == '\0')
  {
    *cursor = NULL;
  }
  else
  {
    *end = '\0';
    *cursor = end + 1;
  }

  return start;
}

BOOL parse_uint32(const char *s, uint32_t *out)
{
  char *end;
  unsigned long value;

  if (s == NULL || out == NULL)
  {
    return FALSE;
  }

  errno = 0;
  value = strtoul(s, &end, 10);

  if (errno == ERANGE)
  {
    return FALSE; // too large for unsigned long
  }

  if (end == s || *end != '\0')
  {
    return FALSE; // no digits, or trailing junk
  }

  if (value > UINT32_MAX)
  {
    return FALSE; // too large for uint32_t
  }

  *out = (uint32_t)value;
  return TRUE; // success
}
