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
#include <btree.h>
#include <btreemeta.h>

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
  void* node = get_page(table->pager, table->root_page_num);
  if (*leaf_node_num_cells(node) >= LEAF_NODE_MAX_CELLS)
  {
    return EXECUTE_TABLE_FULL;
  }

  Row* row_to_insert = &(statement->row_to_insert);

  Cursor cursor;
  init_table_end_cursor(table, &cursor);

  leaf_node_insert(&cursor, row_to_insert->id, row_to_insert);

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

void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value)
{
  void* node = get_page(cursor->table->pager, cursor->page_num);

  uint32_t num_cells = *leaf_node_num_cells(node);
  if (num_cells >= LEAF_NODE_MAX_CELLS)
  {
    // Node full
    printf("Need to implement splitting a leaf node.\n");
    exit(EXIT_FAILURE);
  }

  // Shift cells if inserting into the middle of a leaf node
  if (cursor->cell_num < num_cells)
  {
    // Make room for new cell
    for (uint32_t i = num_cells; i > cursor->cell_num; i--)
    {
      memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i - 1),
             LEAF_NODE_CELL_SIZE);
    }
  }

  *(leaf_node_num_cells(node)) += 1;
  *(leaf_node_key(node, cursor->cell_num)) = key;
  serialize_row(value, leaf_node_value(node, cursor->cell_num));
}
