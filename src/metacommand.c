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

void print_leaf_node(void* node)
{
  printf("Tree:\n");
  uint32_t num_cells = *leaf_node_num_cells(node);
  printf("leaf (size %d)\n", num_cells);
  for (uint32_t i = 0; i < num_cells; i++)
  {
    uint32_t key = *leaf_node_key(node, i);
    printf("  - %d : %d\n", i, key);
  }
}
