#ifndef SIMPLEDB_PAGER_H
#define SIMPLEDB_PAGER_H

#include <stdint.h>

#include <row.h>

enum
{
  MAX_PAGES = 100,
  PAGE_SIZE = 4096,
  ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE,
  TABLE_MAX_ROWS = ROWS_PER_PAGE * MAX_PAGES
};

typedef struct
{
  int file_descriptor;
  uint32_t file_length;
  void* pages[MAX_PAGES];
} Pager;

Pager* pager_open(const char* filename);
void* get_page(Pager* pager, uint32_t page_num);
void pager_flush(Pager* pager, uint32_t page_num, uint32_t size);

#endif
