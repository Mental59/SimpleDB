#ifndef SIMPLEDB_DEFINES_H
#define SIMPLEDB_DEFINES_H

#define TRUE 1
#define FALSE 0
#define BOOL int

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define SIZE_OF_ATTRIBUTE(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

#endif /* SIMPLEDB_DEFINES_H */
