#ifndef SIMPLEDB_UTILS_H
#define SIMPLEDB_UTILS_H

#include <defines.h>

char* next_token(char** cursor, const char* delims);
BOOL parse_uint32(const char* s, uint32_t* out);

#endif
