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
#include "shem.h"

#define AccessPerms 0666
#define AccessPermsSemp 0644

void report_and_exit(const char *msg)
{
  perror(msg);
  exit(-1);
}

int main()
{
  pid_t process_id = getpid();

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  struct timeval millisec;

  gettimeofday(&millisec, NULL);

  sem_unlink(SemRead);
  sem_unlink(SemWrite);

  printf("[%d:C][%02d:%02d:%02d:%03ld][](Server has been started)\n", process_id, tm.tm_hour, tm.tm_min, tm.tm_sec, millisec.tv_usec);

  sem_t *semread = sem_open(SemRead,       /* name */
                            O_CREAT, 0644, /* protection perms */
                            0);            /* initial value */

  sem_t *semwrite = sem_open(SemWrite, /* name */
                             O_CREAT, 0644,
                             1); /* initial value */

  if (semread == (void *)-1)
    report_and_exit("sem_open");

  if (semwrite == (void *)-1)
    report_and_exit("sem_open");

  const char *file = "./fifo";
  mkfifo(file, 0666); /* read/write for user/group/others */

  int fd = open(file, O_RDONLY | O_CREAT);
  if (fd < 0)
    return -1; /* no point in continuing */

  int buffer;
  sem_wait(semread);
  printf("[%d:C][%02d:%02d:%02d:%03ld][](Server trying to read message from shared memory)\n", process_id, tm.tm_hour, tm.tm_min, tm.tm_sec, millisec.tv_usec);

  while (read(fd, &buffer, 1) > 0)
  {
    write(STDOUT_FILENO, &buffer, sizeof(buffer));
  }

  sem_close(semread);
  sem_close(semwrite);
  close(fd);    /* close pipe from read end */
  unlink(file); /* unlink from the underlying file */
  return 0;
}
