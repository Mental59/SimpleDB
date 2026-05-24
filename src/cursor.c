#include <defines.h>
#include <stdlib.h>
#include <stdio.h>

#include <cursor.h>
#include <btree.h>

void init_table_start_cursor(Table* table, Cursor* cursor)
{
  cursor->table = table;
  cursor->page_num = table->root_page_num;
  cursor->cell_num = 0;

  void* root_node = get_page(table->pager, table->root_page_num);
  uint32_t num_cells = *leaf_node_num_cells(root_node);
  cursor->end_of_table = (num_cells == 0);
}

void init_table_find_cursor(Table* table, uint32_t key, Cursor* cursor)
{
  uint32_t root_page_num = table->root_page_num;
  void* root_node = get_page(table->pager, root_page_num);

  if (get_node_type(root_node) == NODE_LEAF)
  {
    leaf_node_find(table, root_page_num, key, cursor);
  }
  else
  {
    internal_node_find(table, root_page_num, key, cursor);
  }
}

void* cursor_value(Cursor* cursor)
{
  uint32_t page_num = cursor->page_num;

  void* page = get_page(cursor->table->pager, page_num);
  if (!page)
  {
    printf("Failed to get row slot\n");
    exit(EXIT_FAILURE);
  }

  return leaf_node_value(page, cursor->cell_num);
}

void cursor_advance(Cursor* cursor)
{
  uint32_t page_num = cursor->page_num;
  void* node = get_page(cursor->table->pager, page_num);

  if (++(cursor->cell_num) >= *leaf_node_num_cells(node))
  {
    cursor->end_of_table = TRUE;
  }
}
