#include <stdint.h>
#include <stdlib.h>

#include <table.h>

Table *new_table(void)
{
  Table *table = (Table *)malloc(sizeof(Table));

  if (table)
  {
    table->num_rows = 0;
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
    {
      table->pages[i] = NULL;
    }
  }

  return table;
}

void free_table(Table *table)
{
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
  {
    if (table->pages[i] != NULL)
    {
      free(table->pages[i]);
    }
  }
  free(table);
}

void *row_slot(Table *table, uint32_t row_num)
{
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void *page = table->pages[page_num];
  if (!page)
  {
    // Allocate memory only when we try to access page
    page = table->pages[page_num] = malloc(PAGE_SIZE);
    if (!page)
    {
      return 0;
    }
  }
  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;
  return (char *)page + byte_offset;
}
