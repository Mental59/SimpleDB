#ifndef SIMEPLDB_DB_H
#define SIMEPLDB_DB_H

#include <stdint.h>

#include <row.h>
#include <pager.h>

typedef struct Table
{
  Pager* pager;
  uint32_t root_page_num;
} Table;

Table* db_open(const char* filename);
void db_close(Table* table);

#endif
