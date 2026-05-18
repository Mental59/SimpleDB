#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pager.h>

Pager* pager_open(const char* filename)
{
  FILE* file = fopen(filename, "r+b");
  if (file == NULL)
  {
    file = fopen(filename, "w+b");
  }

  if (file == NULL)
  {
    printf("Unable to open pager file\n");
    return NULL;
  }

  Pager* pager = (Pager*)malloc(sizeof(Pager));
  if (!pager)
  {
    printf("Failed to allocate pager\n");
    fclose(file);
    return NULL;
  }

  pager->file = file;

  if (fseek(file, 0, SEEK_END) != 0)
  {
    printf("Unable to find pager file length\n");
    fclose(file);
    free(pager);
    return NULL;
  }

  long file_length = ftell(file);
  if (file_length == -1)
  {
    printf("Unable to find pager file length\n");
    fclose(file);
    free(pager);
    return NULL;
  }
  pager->file_length = (uint32_t)file_length;

  for (uint32_t i = 0; i < MAX_PAGES; i++)
  {
    pager->pages[i] = NULL;
  }

  return pager;
}

void* get_page(Pager* pager, uint32_t page_num)
{
  if (page_num > MAX_PAGES)
  {
    printf("Tried to fetch page number out of bounds. %d > %d\n", page_num,
           MAX_PAGES);
    exit(EXIT_FAILURE);
  }

  if (pager->pages[page_num] == NULL)
  {
    // Cache miss. Allocate memory and load from file.
    void* page = malloc(PAGE_SIZE);
    if (!page)
    {
      printf("Failed to allocate page\n");
      exit(EXIT_FAILURE);
    }

    uint32_t num_pages = pager->file_length / PAGE_SIZE;

    // We might save a partial page at the end of the file
    if (pager->file_length % PAGE_SIZE)
    {
      num_pages += 1;
    }

    if (page_num <= num_pages)
    {
      if (fseek(pager->file, (long)(page_num * PAGE_SIZE), SEEK_SET) != 0)
      {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
      }

      size_t bytes_read = fread(page, 1, PAGE_SIZE, pager->file);
      if (bytes_read < PAGE_SIZE && ferror(pager->file))
      {
        printf("Error reading file: %d\n", errno);
        exit(EXIT_FAILURE);
      }
    }

    pager->pages[page_num] = page;
  }

  return pager->pages[page_num];
}

void pager_flush(Pager* pager, uint32_t page_num, uint32_t size)
{
  if (pager->pages[page_num] == NULL)
  {
    printf("Tried to flush null page\n");
    exit(EXIT_FAILURE);
  }

  if (fseek(pager->file, (long)(page_num * PAGE_SIZE), SEEK_SET) != 0)
  {
    printf("Error seeking: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  size_t bytes_written = fwrite(pager->pages[page_num], 1, size, pager->file);

  if (bytes_written < size)
  {
    printf("Error writing: %d\n", errno);
    exit(EXIT_FAILURE);
  }
}
