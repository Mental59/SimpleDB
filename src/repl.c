#include <repl.h>
#include <stdio.h>
#include <string.h>

#include <defines.h>
#include <input.h>
#include <meta.h>
#include <statement.h>
#include <table.h>

static void print_prompt(void)
{
  printf("db > ");
}

BOOL repl_run(void)
{
  Table *table = new_table();
  InputBuffer *input_buffer = new_input_buffer();

  if (!input_buffer)
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
        free_table(table);
        free_input_buffer(input_buffer);
        return TRUE;
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
    case PREPARE_UNRECOGNIZED_STATEMENT:
      printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
      continue;
    }

    switch (execute_statement(&statement, table))
    {
    case (EXECUTE_SUCCESS):
      printf("Executed.\n");
      break;
    case (EXECUTE_TABLE_FULL):
      printf("Error: Table full.\n");
      break;
    }
  }
}
