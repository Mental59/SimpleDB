#include <string.h>

#include <meta.h>

MetaCommandResult decode_meta_command(char* command)
{
  if (strcmp(command, ".exit") == 0)
  {
    return META_COMMAND_EXIT;
  }

  return META_COMMAND_UNRECOGNIZED;
}
