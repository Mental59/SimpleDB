#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include <statement.h>
#include <input.h>
#include <row.h>
#include <defines.h>
#include <utils.h>
#include <cursor.h>

static void print_row(Row* row)
{
  printf("(%u, %s, %s)\n", row->id, row->username, row->email);
}

PrepareResult prepare_statement(char* buffer, Statement* statement)
{
  if (strncmp(buffer, "insert ", 7) == 0)
  {
    return prepare_insert(buffer, statement);
  }

  if (strcmp(buffer, "select") == 0)
  {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_statement(Statement* statement, Table* table)
{
  switch (statement->type)
  {
  case (STATEMENT_INSERT):
    return execute_insert(statement, table);
  case (STATEMENT_SELECT):
    return execute_select(statement, table);
  }
}

ExecuteResult execute_insert(Statement* statement, Table* table)
{
  if (table->num_rows >= TABLE_MAX_ROWS)
  {
    return EXECUTE_TABLE_FULL;
  }

  Row* row_to_insert = &(statement->row_to_insert);

  Cursor cursor;
  init_table_end_cursor(table, &cursor);

  serialize_row(row_to_insert, cursor_value(&cursor));
  table->num_rows += 1;

  return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement, Table* table)
{
  Row row;
  Cursor cursor;

  init_table_start_cursor(table, &cursor);

  for (; !cursor.end_of_table; cursor_advance(&cursor))
  {
    deserialize_row(cursor_value(&cursor), &row);
    print_row(&row);
  }

  return EXECUTE_SUCCESS;
}

PrepareResult prepare_insert(char* buffer, Statement* statement)
{
  statement->type = STATEMENT_INSERT;

  char* buffer_copy = malloc(strlen(buffer) + 1);
  if (buffer_copy == NULL)
  {
    printf(
        "Failed to prepare insert statement, buffer copy allocation failed\n");
    return PREPARE_FAILURE;
  }

  strcpy(buffer_copy, buffer);

  char* cursor = buffer_copy;
  const char* delims = " ";

  next_token(&cursor, delims); // skip insert keyword
  char* id_string = next_token(&cursor, delims);
  char* username = next_token(&cursor, delims);
  char* email = next_token(&cursor, delims);

  if (id_string == NULL || username == NULL || email == NULL)
  {
    free(buffer_copy);
    return PREPARE_SYNTAX_ERROR;
  }

  uint32_t id;
  if (!parse_uint32(id_string, &id))
  {
    free(buffer_copy);
    return PREPARE_INVALID_UINT32;
  }
  if (strlen(username) > COLUMN_USERNAME_SIZE)
  {
    free(buffer_copy);
    return PREPARE_STRING_TOO_LONG;
  }
  if (strlen(email) > COLUMN_EMAIL_SIZE)
  {
    free(buffer_copy);
    return PREPARE_STRING_TOO_LONG;
  }

  statement->row_to_insert.id = id;
  strcpy(statement->row_to_insert.username, username);
  strcpy(statement->row_to_insert.email, email);

  free(buffer_copy);
  return PREPARE_SUCCESS;
}
