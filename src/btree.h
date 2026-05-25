#ifndef SIMPLEDB_BTREE_H
#define SIMPLEDB_BTREE_H

#include <stdint.h>

#include <defines.h>

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
void initialize_internal_node(void* node);
NodeType get_node_type(void* node);
void set_node_type(void* node, NodeType type);
BOOL is_node_root(void* node);
void set_node_root(void* node, BOOL is_root);

uint32_t* internal_node_num_keys(void* node);
uint32_t* internal_node_right_child(void* node);
uint32_t* internal_node_cell(void* node, uint32_t cell_num);
uint32_t* internal_node_child(void* node, uint32_t child_num);
uint32_t* internal_node_key(void* node, uint32_t key_num);

void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value);
void leaf_node_find(Table* table, uint32_t page_num, uint32_t key,
                    Cursor* cursor);
void leaf_node_split_and_insert(Cursor* cursor, uint32_t key, Row* value);
uint32_t* leaf_node_next_leaf(void* node);

void create_new_root(Table* table, uint32_t right_child_page_num);

uint32_t get_node_max_key(void* node);

void internal_node_find(Table* table, uint32_t page_num, uint32_t key,
                        Cursor* cursor);

#endif
