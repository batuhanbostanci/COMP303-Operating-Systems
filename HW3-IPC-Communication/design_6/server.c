#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include "socket.h"
#include <semaphore.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void report_and_terminate(const char *message, int terminate)
{
    perror(message);
    if (terminate)
    {
        exit(-1);
    }
}
void report_and_exit(const char *msg)
{
    perror(msg);
    exit(-1);
}

int main()
{
    int fd = socket(AF_INET,     /* network versus AF_LOCAL */
                    SOCK_STREAM, /* reliable, bidirectional: TCP */
                    0);          /* system picks underlying protocol */
    if (fd < 0)
        report_and_terminate("socket", 1); /* terminate */

    /* bind the server's local address in memory */
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));          /* clear the bytes */
    saddr.sin_family = AF_INET;                /* versus AF_LOCAL */
    saddr.sin_addr.s_addr = htonl(INADDR_ANY); /* host-to-network endian */
    saddr.sin_port = htons(PortNumber);        /* for listening */

    if (bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
        report_and_terminate("bind", 1); /* terminate */

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

    /* listen to the socket */
    if (listen(fd, MaxConnects) < 0)       /* listen for clients, up to MaxConnects */
        report_and_terminate("listen", 1); /* terminate */

    fprintf(stderr, "Server is listening %i the clients...\n", PortNumber);
    /* a server traditionally listens indefinitely */
    while (1)
    {
        struct sockaddr_in caddr; /* client address */
        int len = sizeof(caddr);  /* address length could change */

        int client_fd = accept(fd, (struct sockaddr *)&caddr, &len); /* accept blocks */
        if (client_fd < 0)
        {
            report_and_terminate("accept", 0); /* don't terminated, though there's a problem */
            continue;
        }

        /* read from client */
        int i;

        char buffer[BufferSize + 1];
        memset(buffer, '\0', sizeof(buffer));
        int count = read(client_fd, buffer, sizeof(buffer));
        if (count > 0)
        {
            puts(buffer);
        }

        close(client_fd); /* break connection */
    }                     /* while(1) */

    return 0;
}
