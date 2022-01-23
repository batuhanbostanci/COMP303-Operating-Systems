/** Compilation: gcc -o memwriter memwriter.c -lrt -lpthread **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "shmem.h"

void report_and_exit(const char *msg)
{
  perror(msg);
  exit(-1);
}

// void writer(char *message){
//   int fd = shm_open(BackingFile,      /* name from smem.h */
// 		    O_RDWR | O_CREAT, /* read/write, create if needed */
// 		    AccessPerms);     /* access permissions (0644) */
//   if (fd < 0) report_and_exit("Can't open shared mem segment...");

//   ftruncate(fd, ByteSize); /* get the bytes */

//   caddr_t memptr = mmap(NULL,       /* let system pick where to put segment */
// 			ByteSize,   /* how many bytes */
// 			PROT_READ | PROT_WRITE, /* access protections */
// 			MAP_SHARED, /* mapping visible to other processes */
// 			fd,         /* file descriptor */
// 			0);         /* offset: start at 1st byte */
//   if ((caddr_t) -1  == memptr) report_and_exit("Can't get segment...");

//   /* semahore code to lock the shared mem */
//   sem_t* semptr = sem_open(SemaphoreName, /* name */
// 			   O_CREAT,       /* create the semaphore */
// 			   AccessPerms,   /* protection perms */
// 			   0);            /* initial value */
//   if (semptr == (void*) -1) report_and_exit("sem_open");

//   strcpy(memptr,message); /* copy some ASCII bytes to the segment */

//   /* increment the semaphore so that memreader can read */
//   if (sem_post(semptr) < 0) report_and_exit("sem_post");

//   /* clean up */
//   munmap(memptr, ByteSize); /* unmap the storage */
//   close(fd);
//   //sem_close(semptr);
//   // shm_unlink(BackingFile); /* unlink from the backing file */

// }

int main()
{

  char client_name[50];
  char copy_client_name[50];
  char client_message[50];
  char copy_client_message[50];
  int answer_in_int;

  pid_t process_id = getpid();

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  struct timeval millisec;

  gettimeofday(&millisec, NULL);

  printf("[%d:C][%02d:%02d:%02d:%03ld][](Client has been started)\n", process_id, tm.tm_hour, tm.tm_min, tm.tm_sec, millisec.tv_usec);
  printf("Creating a client, please press 1. To exit, please press 2\n");
  scanf("%d", &answer_in_int);

  int fd = shm_open(BackingFile, O_RDWR | O_CREAT, AccessPerms);
  if (fd < 0)
    report_and_exit("Can't open shared mem segment...");

  ftruncate(fd, ByteSize);

  caddr_t memptr = mmap(NULL, ByteSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if ((caddr_t)-1 == memptr)
    report_and_exit("Can't get segment...");

  /* create a semaphore for mutual exclusion */
  sem_t *semptw = sem_open(SemaphoreWriter, /* name */
                           O_CREAT,         /* create the semaphore */
                           AccessPerms,     /* protection perms */
                           0);              /* initial value */

  if (semptw == (void *)-1)
    report_and_exit("sem_open");

  printf("Creating a client, please press 1. To exit, please press 2\n");
  scanf("%d", &answer_in_int);

  while (!sem_wait(semptw))
  {
    if (answer_in_int == 1)
    {
      sem_t *semptr = sem_open(SemaphoreName,
                               O_CREAT,     /* create the semaphore */
                               AccessPerms, /* protection perms */
                               0);          /* initial value */

      struct tm tm1 = *localtime(&t);
      struct timeval millisec1;
      gettimeofday(&millisec1, NULL);

      printf("[%d:C][%02d:%02d:%02d:%03ld][] Please enter your name\n", process_id, tm1.tm_hour, tm1.tm_min, tm1.tm_sec, millisec1.tv_usec);
      scanf("%s", client_name);
      strcpy(copy_client_name, client_name);

      struct tm tm2 = *localtime(&t);
      struct timeval millisec2;
      gettimeofday(&millisec2, NULL);

      printf("[%d:C][%02d:%02d:%02d:%03ld][%s](Client name is set to “%s”)\n", process_id, tm2.tm_hour, tm2.tm_min, tm2.tm_sec, millisec2.tv_usec, copy_client_name, copy_client_name);

      struct tm tm3 = *localtime(&t);
      struct timeval millisec3;
      gettimeofday(&millisec3, NULL);

      printf("\n[%d:C][%02d:%02d:%02d:%03ld][%s] Please enter your note\n", process_id, tm3.tm_hour, tm3.tm_min, tm3.tm_sec, millisec3.tv_usec, client_name);
      scanf(" %[^\t\n]s", client_message);
      strcpy(copy_client_message, client_message);
      char mess[] = "\n";
      strcat(copy_client_message, mess);
      strcpy(memptr, copy_client_message);
      //writer(copy_client_message);

      struct tm tm4 = *localtime(&t);
      struct timeval millisec4;
      gettimeofday(&millisec4, NULL);
      printf("[%d:C][%02d:%02d:%02d:%03ld][%s] Trying to send message\n", process_id, tm4.tm_hour, tm4.tm_min, tm4.tm_sec, millisec4.tv_usec, client_name);

      sem_post(semptr);
    }

    printf("Creating a client, please press 1. To exit, please press 2\n");
    scanf("%d", &answer_in_int);

    if (answer_in_int == 2)
    {
      break;
    }
  }

  return 0;
}
