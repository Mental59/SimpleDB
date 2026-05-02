#include <stdlib.h>
#include <repl.h>

int main(void)
{
  return repl_run() ? EXIT_SUCCESS : EXIT_FAILURE;
}
