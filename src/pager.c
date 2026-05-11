#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pager.h>

Pager *pager_open(const char *filename)
{
  int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

  if (fd == -1)
  {
    printf("Unable to open pager file\n");
    return NULL;
  }

  Pager *pager = (Pager *)malloc(sizeof(Pager));
  if (!pager)
  {
    printf("Failed to allocate pager\n");
    return NULL;
  }

  pager->file_descriptor = fd;
  pager->file_length = lseek(fd, 0, SEEK_END);

  for (uint32_t i = 0; i < MAX_PAGES; i++)
  {
    pager->pages[i] = NULL;
  }

  return pager;
}

void *get_page(Pager *pager, uint32_t page_num)
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
    void *page = malloc(PAGE_SIZE);
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
      lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
      ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
      if (bytes_read == -1)
      {
        printf("Error reading file: %d\n", errno);
        exit(EXIT_FAILURE);
      }
    }

    pager->pages[page_num] = page;
  }

  return pager->pages[page_num];
}

void pager_flush(Pager *pager, uint32_t page_num, uint32_t size)
{
  if (pager->pages[page_num] == NULL)
  {
    printf("Tried to flush null page\n");
    exit(EXIT_FAILURE);
  }

  off_t offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);

  if (offset == -1)
  {
    printf("Error seeking: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written =
      write(pager->file_descriptor, pager->pages[page_num], size);

  if (bytes_written == -1)
  {
    printf("Error writing: %d\n", errno);
    exit(EXIT_FAILURE);
  }
}
