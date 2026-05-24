#ifndef SIMPLEDB_METACOMMAND_H
#define SIMPLEDB_METACOMMAND_H

#include <stdint.h>

typedef struct Pager Pager;

typedef enum
{
  META_COMMAND_EXIT,
  META_COMMAND_CONSTANTS,
  META_COMMAND_BTREE,
  META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

MetaCommandResult decode_meta_command(char* command);

void print_constants(void);
void indent(uint32_t level);
void print_tree(Pager* pager, uint32_t page_num, uint32_t indentation_level);

#endif
