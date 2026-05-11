#ifndef SIMEPLDB_TABLE_H
#define SIMEPLDB_TABLE_H

#include <stdint.h>

#include <row.h>
#include <pager.h>

typedef struct
{
  uint32_t num_rows;
  Pager *pager;
} Table;

Table *db_open(const char *filename);
void db_close(Table *table);
void *row_slot(Table *table, uint32_t row_num);

#endif
