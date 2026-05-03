#ifndef SIMPLEDB_STATEMENT_H
#define SIMPLEDB_STATEMENT_H

#include <stdint.h>

#include <row.h>
#include <table.h>

typedef enum
{
  PREPARE_SUCCESS,
  PREPARE_SYNTAX_ERROR,
  PREPARE_STRING_TOO_LONG,
  PREPARE_INVALID_UINT32,
  PREPARE_FAILURE,
  PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum
{
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

typedef enum
{
  EXECUTE_SUCCESS,
  EXECUTE_TABLE_FULL
} ExecuteResult;

typedef struct
{
  StatementType type;
  Row row_to_insert;
} Statement;

PrepareResult prepare_statement(char *buffer, Statement *statement);
ExecuteResult execute_statement(Statement *statement, Table *table);

ExecuteResult execute_insert(Statement *statement, Table *table);
ExecuteResult execute_select(Statement *statement, Table *table);

PrepareResult prepare_insert(char *buffer, Statement *statement);

#endif
