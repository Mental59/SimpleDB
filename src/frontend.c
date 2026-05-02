#include <string.h>

#include <frontend.h>
#include <input.h>

PrepareResult prepare_statement(char *buffer, Statement *statement)
{
  if (strncmp(buffer, "insert", 6) == 0)
  {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }

  if (strcmp(buffer, "select") == 0)
  {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement *statement)
{
  switch (statement->type)
  {
  case (STATEMENT_INSERT):
    printf("This is where we would do an insert.\n");
    break;
  case (STATEMENT_SELECT):
    printf("This is where we would do a select.\n");
    break;
  }
}
