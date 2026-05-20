#ifndef SIMPLEDB_METACOMMAND_H
#define SIMPLEDB_METACOMMAND_H

typedef enum
{
  META_COMMAND_EXIT,
  META_COMMAND_CONSTANTS,
  META_COMMAND_BTREE,
  META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

MetaCommandResult decode_meta_command(char* command);

void print_constants(void);
void print_leaf_node(void* node);

#endif
