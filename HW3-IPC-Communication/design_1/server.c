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

void file_read()
{

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd;
    if ((fd = open(FileName, O_RDONLY)) < 0)
        report_and_exit("open to read failed...");

    fcntl(fd, F_GETLK, &lock);
    if (lock.l_type != F_UNLCK)
        report_and_exit("file is still write locked...");
    lock.l_type = F_RDLCK;
    if (fcntl(fd, F_SETLK, &lock) < 0)
        report_and_exit("can't get a read-only lock...");

    int c;
    while (read(fd, &c, 1) > 0)
        write(STDOUT_FILENO, &c, 1);

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) < 0)
        report_and_exit("explicit unlocking failed...");
    close(fd);
}
void file_write()
{
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int fd;
    if ((fd = open(FileName, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
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
        char empty[] = "";
        write(fd, empty, strlen(empty));
    }

    lock.l_type = F_UNLCK;

    if (fcntl(fd, F_SETLK, &lock) < 0)
        report_and_exit("explicit unlocking failed...\n");

    close(fd);
}

int main()
{

    char client_message[50];
    char copy_client_message[50];
    int answer_in_int;

    pid_t process_id = getpid();

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    struct timeval millisec;

    gettimeofday(&millisec, NULL);

    printf("[%d:C][%02d:%02d:%02d:%03ld][](Server has been started)\n", process_id, tm.tm_hour, tm.tm_min, tm.tm_sec, millisec.tv_usec);

    while (1)
    {
        file_read();
        file_write();
        usleep(30000);
    }

    return 0;
}
