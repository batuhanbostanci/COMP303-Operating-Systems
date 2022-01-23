#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#define FileName "desing_1.dat"

void report_and_exit(const char *msg)
{
  perror(msg);
  exit(-1);
}

void client(char *message)
{
}

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

  while (1)
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

    struct tm tm4 = *localtime(&t);
    struct timeval millisec4;
    gettimeofday(&millisec4, NULL);
    printf("[%d:C][%02d:%02d:%02d:%03ld][%s] Trying to send message\n", process_id, tm4.tm_hour, tm4.tm_min, tm4.tm_sec, millisec4.tv_usec, client_name);

    /***************/
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int fd;
    if ((fd = open(FileName, O_RDWR | O_CREAT, 0666)) < 0)
      report_and_exit("open to read failed...\n");

    fcntl(fd, F_GETLK, &lock);

    if (lock.l_type != F_UNLCK)
      report_and_exit("file is still write locked...\n");

    lock.l_type = F_RDLCK;

    if (fcntl(fd, F_SETLK, &lock) < 0)
    {
      report_and_exit("can't get a read-only lock...\n");
    }
    else
    {
      char message[] = "\n";
      write(fd, message, strlen(message));

      write(fd, copy_client_message, strlen(copy_client_message));
    }

    lock.l_type = F_UNLCK;

    if (fcntl(fd, F_SETLK, &lock) < 0)
      report_and_exit("explicit unlocking failed...\n");

    close(fd);
    /***************/

    printf("Creating a client, please press 1. To exit, please press 2\n");
    scanf("%d", &answer_in_int);

    if (answer_in_int == 2)
    {
      break;
    }
  }

  return 0;
}
