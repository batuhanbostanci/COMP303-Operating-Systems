
/** Compilation: gcc -o memreader memreader.c -lrt -lpthread **/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "que.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

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

  key_t key = ftok(PathName, ProjectId);
  if (key < 0)
  {
    report_and_exit("Error to get key.");
  }

  int qid = msgget(key, 0666 | IPC_CREAT);
  if (qid < 0)
  {
    report_and_exit("Error to get id of queue.");
  }

  sem_wait(semread);
  //Read from queue
  for (int i = 0; i < MsgCount; i++)
  {
    queuedMessage msg;
    if (msgrcv(qid, &msg, sizeof(msg), 3, MSG_NOERROR) < 0)
    {
      printf("[%d:C][%02d:%02d:%02d:%03ld][](Server trying to read message from shared memory)\n", process_id, tm.tm_hour, tm.tm_min, tm.tm_sec, millisec.tv_usec);
      exit(-1);
    }
    printf("%s\n", msg.payload);
  }

  if (msgctl(qid, IPC_RMID, NULL) < 0)
  {
    report_and_exit("Error to remove queue.");
  }

  sem_close(semread);
  sem_close(semwrite);

  return 0;
}
