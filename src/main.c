#include <stdlib.h>
#include <stdio.h>

#include <repl.h>

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Must supply a database filename.\n");
    exit(EXIT_FAILURE);
  }

  char *database_filename = argv[1];
  return repl_run(database_filename) ? EXIT_SUCCESS : EXIT_FAILURE;
}
