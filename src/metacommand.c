#include <string.h>
#include <stdio.h>

#include <metacommand.h>
#include <btree.h>
#include <btreemeta.h>

MetaCommandResult decode_meta_command(char* command)
{
  if (strcmp(command, ".exit") == 0)
  {
    return META_COMMAND_EXIT;
  }
  if (strcmp(command, ".constants") == 0)
  {
    return META_COMMAND_CONSTANTS;
  }
  if (strcmp(command, ".btree") == 0)
  {
    return META_COMMAND_BTREE;
  }

  return META_COMMAND_UNRECOGNIZED;
}

void print_constants(void)
{
  printf("Constants:\n");
  printf("ROW_SIZE: %d\n", ROW_SIZE);
  printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
  printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
  printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
  printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
  printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

void indent(uint32_t level)
{
  for (uint32_t i = 0; i < level; i++)
  {
    printf("  ");
  }
}

void print_tree_rec(Pager* pager, uint32_t page_num, uint32_t indentation_level)
{
  void* node = get_page(pager, page_num);
  uint32_t num_keys, child;

  switch (get_node_type(node))
  {
  case (NODE_LEAF):
    num_keys = *leaf_node_num_cells(node);
    indent(indentation_level);
    printf("- leaf (size %d)\n", num_keys);
    for (uint32_t i = 0; i < num_keys; i++)
    {
      indent(indentation_level + 1);
      printf("- %d\n", *leaf_node_key(node, i));
    }
    break;
  case (NODE_INTERNAL):
    num_keys = *internal_node_num_keys(node);
    indent(indentation_level);
    printf("- internal (size %d)\n", num_keys);
    for (uint32_t i = 0; i < num_keys; i++)
    {
      child = *internal_node_child(node, i);
      print_tree_rec(pager, child, indentation_level + 1);

      indent(indentation_level + 1);
      printf("- key %d\n", *internal_node_key(node, i));
    }
    child = *internal_node_right_child(node);
    print_tree_rec(pager, child, indentation_level + 1);
    break;
  }
}

void print_tree(Pager* pager, uint32_t page_num, uint32_t indentation_level)
{
  printf("Tree:\n");
  print_tree_rec(pager, page_num, indentation_level);
}
