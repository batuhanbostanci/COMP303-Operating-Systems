/** Compilation: gcc -o memreader memreader.c -lrt -lpthread **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "shmem.h"

void report_and_exit(const char *msg)
{
  perror(msg);
  exit(-1);
}
void reader()
{
  int fd = shm_open(BackingFile, O_RDWR | O_CREAT, AccessPerms); /* empty to begin */
  if (fd < 0)
    report_and_exit("Can't get file descriptor...");

  /* get a pointer to memory */
  caddr_t memptr = mmap(NULL,                   /* let system pick where to put segment */
                        ByteSize,               /* how many bytes */
                        PROT_READ | PROT_WRITE, /* access protections */
                        MAP_SHARED,             /* mapping visible to other processes */
                        fd,                     /* file descriptor */
                        0);                     /* offset: start at 1st byte */
  if ((caddr_t)-1 == memptr)
    report_and_exit("Can't access segment...");

  /* create a semaphore for mutual exclusion */
  sem_t *semptw = sem_open(SemaphoreWriter, /* name */
                           O_CREAT,         /* create the semaphore */
                           AccessPerms,     /* protection perms */
                           0);              /* initial value */

  if (semptw == (void *)-1)
    report_and_exit("sem_open");

  int i;
  for (i = 0; i < 512; i++)
    write(STDOUT_FILENO, memptr + i, 1); /* one byte at a time */
  sem_post(semptw);

  munmap(memptr, ByteSize);
  close(fd);
}

int main()
{
  pid_t process_id = getpid();

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  struct timeval millisec;

  gettimeofday(&millisec, NULL);

  printf("[%d:C][%02d:%02d:%02d:%03ld][](Server has been started)\n", process_id, tm.tm_hour, tm.tm_min, tm.tm_sec, millisec.tv_usec);

  sem_t *semptr = sem_open(SemaphoreName, /* name */
                           O_CREAT,       /* create the semaphore */
                           AccessPerms,   /* protection perms */
                           0);            /* initial value */

  if (semptr == (void *)-1)
    report_and_exit("sem_open");

  sem_post(semptr);
  while (!sem_wait(semptr))
  {
    printf("[%d:C][%02d:%02d:%02d:%03ld][](Server trying to read message from shared memory)\n", process_id, tm.tm_hour, tm.tm_min, tm.tm_sec, millisec.tv_usec);
    reader();
  }

  return 0;
}
