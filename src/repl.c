#include <repl.h>
#include <stdio.h>
#include <string.h>

#include <defines.h>
#include <input.h>
#include <metacommand.h>
#include <statement.h>
#include <db.h>

static void print_prompt(void)
{
  printf("db > ");
}

BOOL repl_run(const char* database_filename)
{
  Table* table = db_open(database_filename);
  InputBuffer* input_buffer = new_input_buffer();

  if (!input_buffer || !table)
  {
    return FALSE;
  }

  while (TRUE)
  {
    print_prompt();
    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.')
    {
      switch (decode_meta_command(input_buffer->buffer))
      {
      case (META_COMMAND_EXIT):
        db_close(table);
        free_input_buffer(input_buffer);
        return TRUE;
      case (META_COMMAND_CONSTANTS):
        print_constants();
        continue;
      case (META_COMMAND_BTREE):
        print_leaf_node(get_page(table->pager, 0));
        continue;
      case (META_COMMAND_UNRECOGNIZED):
        printf("Unrecognized command '%s'\n", input_buffer->buffer);
        continue;
      }
    }

    Statement statement;
    switch (prepare_statement(input_buffer->buffer, &statement))
    {
    case PREPARE_SUCCESS:
      break;
    case PREPARE_SYNTAX_ERROR:
      printf("Syntax error. Could not parse statement.\n");
      continue;
    case PREPARE_STRING_TOO_LONG:
      printf("String is too long.\n");
      continue;
    case PREPARE_INVALID_UINT32:
      printf("Invalid uint32 value.\n");
      continue;
    case PREPARE_FAILURE:
      printf("Unexpected error.\n");
      continue;
    case PREPARE_UNRECOGNIZED_STATEMENT:
      printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
      continue;
    }

    switch (execute_statement(&statement, table))
    {
    case (EXECUTE_SUCCESS):
      printf("Executed.\n");
      break;
    case (EXECUTE_DUPLICATE_KEY):
      printf("Error: Duplicate key.\n");
      break;
    case (EXECUTE_TABLE_FULL):
      printf("Error: Table full.\n");
      break;
    }
  }
}
