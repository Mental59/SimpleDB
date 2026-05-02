#ifndef SIMPLEDB_META_H
#define SIMPLEDB_META_H

typedef enum
{
  META_COMMAND_EXIT,
  META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

MetaCommandResult decode_meta_command(char *command);

#endif
