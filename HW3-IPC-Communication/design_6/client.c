#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include "socket.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ipc.h>

#define AccessPerms 0666
#define AccessPermsSemp 0644

void report(const char *msg, int terminate)
{
  perror(msg);
  if (terminate)
    exit(-1); /* failure */
}
void report_and_exit(const char *msg)
{
  perror(msg);
  exit(-1);
}

int main()
{
  char client_name[50];
  char copy_client_name[50];
  char client_message[50];
  char copy_client_message[50];
  int answer_in_int;

  /* fd for the socket */
  int sockfd = socket(AF_INET,     /* versus AF_LOCAL */
                      SOCK_STREAM, /* reliable, bidirectional */
                      0);          /* system picks protocol (TCP) */
  if (sockfd < 0)
    report("socket", 1); /* terminate */

  /* get the address of the host */
  struct hostent *hptr = gethostbyname(Host); /* localhost: 127.0.0.1 */
  if (!hptr)
    report("gethostbyname", 1);    /* is hptr NULL? */
  if (hptr->h_addrtype != AF_INET) /* versus AF_LOCAL */
    report("bad address family", 1);

  /* connect to the server: configure server's address 1st */
  struct sockaddr_in saddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr =
      ((struct in_addr *)hptr->h_addr_list[0])->s_addr;
  saddr.sin_port = htons(PortNumber); /* port number in big-endian */

  if (connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    report("connect", 1);

  sem_t *semread = sem_open(SemRead,     /* name */
                            O_CREAT,     /* create the semaphore */
                            AccessPerms, /* protection perms */
                            0);          /* initial value */

  sem_t *semwrite = sem_open(SemWrite,    /* name */
                             O_CREAT,     /* create the semaphore */
                             AccessPerms, /* protection perms */
                             0);          /* initial value */
  if (semread == (void *)-1)
    report_and_exit("sem_open");

  if (semwrite == (void *)-1)
    report_and_exit("sem_open");

  pid_t process_id = getpid();

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  struct timeval millisec;

  gettimeofday(&millisec, NULL);

  printf("[%d:C][%02d:%02d:%02d:%03ld][](Client has been started)\n", process_id, tm.tm_hour, tm.tm_min, tm.tm_sec, millisec.tv_usec);

  printf("Creating a client, please press 1. To exit, please press 2\n");
  scanf("%d", &answer_in_int);

  while (1)
  {
    if (answer_in_int == 1)
    {

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
      sem_post(semwrite);
      if (!sem_wait(semwrite))
      {
        struct tm tm4 = *localtime(&t);
        struct timeval millisec4;
        gettimeofday(&millisec4, NULL);
        printf("[%d:C][%02d:%02d:%02d:%03ld][%s] Trying to send message\n", process_id, tm4.tm_hour, tm4.tm_min, tm4.tm_sec, millisec4.tv_usec, client_name);

        if (write(sockfd, copy_client_message, strlen(copy_client_message)) > 0)
        {
          char buff[BufferSize + 1];
          memset(buff, '\0', sizeof(buff));
        }
        sem_post(semwrite);
      }
    }

    printf("Creating a client, please press 1. To exit, please press 2\n");
    scanf("%d", &answer_in_int);

    if (answer_in_int == 2)
    {
      break;
    }
  }
  sem_close(semwrite);
  sem_close(semread);

  return 0;
}
