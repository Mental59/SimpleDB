#ifndef SIMPLEDB_CURSOR_H
#define SIMPLEDB_CURSOR_H

#include <table.h>
#include <defines.h>

typedef struct
{
  Table* table;
  uint32_t row_num;
  BOOL end_of_table; // Indicates a position one past the last element
} Cursor;

Cursor* alloc_table_start_cursor(Table* table);
Cursor* alloc_table_end_cursor(Table* table);

void init_table_start_cursor(Table* table, Cursor* cursor);
void init_table_end_cursor(Table* table, Cursor* cursor);

void* cursor_value(Cursor* cursor);
void cursor_advance(Cursor* cursor);

#endif
