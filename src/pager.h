#ifndef SIMPLEDB_PAGER_H
#define SIMPLEDB_PAGER_H

#include <stdint.h>
#include <stdio.h>

#include <row.h>

enum
{
  MAX_PAGES = 100,
  PAGE_SIZE = 4096
};

typedef struct
{
  FILE* file;
  uint32_t file_length;
  uint32_t num_pages;
  void* pages[MAX_PAGES];
} Pager;

Pager* pager_open(const char* filename);
void* get_page(Pager* pager, uint32_t page_num);
void pager_flush(Pager* pager, uint32_t page_num);

#endif
