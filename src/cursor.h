#ifndef SIMPLEDB_CURSOR_H
#define SIMPLEDB_CURSOR_H

#include <db.h>
#include <defines.h>

typedef struct Cursor
{
  Table* table;
  uint32_t page_num;
  uint32_t cell_num;
  BOOL end_of_table; // Indicates a position one past the last element
} Cursor;

void init_table_start_cursor(Table* table, Cursor* cursor);
void init_table_find_cursor(Table* table, uint32_t key, Cursor* cursor);

void* cursor_value(Cursor* cursor);
void cursor_advance(Cursor* cursor);

#endif
