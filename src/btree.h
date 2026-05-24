#ifndef SIMPLEDB_BTREE_H
#define SIMPLEDB_BTREE_H

#include <stdint.h>

typedef enum
{
  NODE_INTERNAL,
  NODE_LEAF
} NodeType;

typedef struct Cursor Cursor;
typedef struct Row Row;
typedef struct Table Table;

uint32_t* leaf_node_num_cells(void* node);
void* leaf_node_cell(void* node, uint32_t cell_num);
uint32_t* leaf_node_key(void* node, uint32_t cell_num);
void* leaf_node_value(void* node, uint32_t cell_num);
void initialize_leaf_node(void* node);
NodeType get_node_type(void* node);
void set_node_type(void* node, NodeType type);

void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value);
void leaf_node_find(Table* table, uint32_t page_num, uint32_t key,
                    Cursor* cursor);

#endif
