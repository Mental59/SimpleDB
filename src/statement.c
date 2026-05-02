#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include <statement.h>
#include <input.h>
#include <row.h>
#include <defines.h>

static void print_row(Row *row)
{
  printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

PrepareResult prepare_statement(char *buffer, Statement *statement)
{
  if (strncmp(buffer, "insert ", 7) == 0)
  {
    statement->type = STATEMENT_INSERT;

    Row *row = &statement->row_to_insert;
    const char *format =
        "insert %" SCNu32
        " %" STR(USERNAME_MAX_CHARS) "s %" STR(EMAIL_MAX_CHARS) "s";
    int args_assigned =
        sscanf(buffer, format, &(row->id), row->username, row->email);
    if (args_assigned < 3)
    {
      return PREPARE_SYNTAX_ERROR;
    }

    return PREPARE_SUCCESS;
  }

  if (strcmp(buffer, "select") == 0)
  {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_statement(Statement *statement, Table *table)
{
  switch (statement->type)
  {
  case (STATEMENT_INSERT):
    return execute_insert(statement, table);
  case (STATEMENT_SELECT):
    return execute_select(statement, table);
  }
}

ExecuteResult execute_insert(Statement *statement, Table *table)
{
  if (table->num_rows >= TABLE_MAX_ROWS)
  {
    return EXECUTE_TABLE_FULL;
  }

  Row *row_to_insert = &(statement->row_to_insert);

  serialize_row(row_to_insert, row_slot(table, table->num_rows));
  table->num_rows += 1;

  return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table)
{
  Row row;
  for (uint32_t i = 0; i < table->num_rows; i++)
  {
    deserialize_row(row_slot(table, i), &row);
    print_row(&row);
  }
  return EXECUTE_SUCCESS;
}
