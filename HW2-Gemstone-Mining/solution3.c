/*
 ============================================================================
 Name        : solution3.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



int main(void) {
	char ch;
	FILE *f;
	int counter,lower,upper;

	counter =0;// 24 char counter
	lower = 500; // Least value for msec.
	upper = 800; // Max value for msec.

	//Count for mines
	int count_diamond =0;
	int count_ruby =0;
	int count_sapphire =0;
	int count_emerald =0;

	//Values for mines
	int value_diamond =3500;
	int value_ruby =50;
	int value_sapphire =1200;
	int value_emerald =800;


	// Define pipes
	int fd1[2];
	int fd2[2];
	pipe(fd1); //for parent
	pipe(fd2); //for child

	char file_name[13] ="buckets/0.txt"; //File name
	int total_bucket_value =0; //Total value of bucket

	if(pipe(fd1) ==-1){// Controlling pipe
		printf("An error occurred with opening the pipe\n");
	}
	if(pipe(fd2) ==-1){// Controlling pipe
			printf("An error occurred with opening the pipe\n");
		}


	int i;// variable for number of forks

	// Creating fork
	int id = fork();
	if(id ==0){
		printf("[PID:%d]A new child has been created with PID:%d\n", getppid(), getpid());
		close(fd2[0]);
		close(fd1[1]);

		for(i=0;i<5;i++){
		// Changing index of file name with real index
		file_name[8] = i +'0';

		f = fopen(file_name, "r");

		// Controlling, is there any error while creating read process
		if(f == NULL){
			printf("Error is occurred\n");
			exit(EXIT_FAILURE);
		}

			//reading char(s) from bucket files
			while((ch =fgetc(f)) != EOF){
				counter = counter + 1;
				if(counter ==24){
					int random = (rand() % (upper - lower+1)) + lower;
					float time = (float)random/1000;
					sleep(time);
					printf("[PID:%d] Tired. Going to sleep for %dmsec.\n", getpid(), random);
					counter =0;
				}

				if(ch == 'd'){
					count_diamond = count_diamond +1;
					printf("[PID:%d] Found %d. Diamond.\n", getpid(),count_diamond);
					write(fd2[1],&value_diamond,sizeof(int));
				}
				else if(ch == 'r'){
					count_ruby = count_ruby +1;
					printf("[PID:%d] Found %d. Ruby.\n", getpid(),count_ruby);
					write(fd2[1],&value_ruby,sizeof(int));
				}
				else if(ch == 's'){
					count_sapphire = count_sapphire +1;
					printf("[PID:%d] Found %d. Sapphire.\n", getpid(),count_sapphire);
					write(fd2[1],&value_sapphire,sizeof(int));
				}
				else if(ch == 'e'){
					count_emerald = count_emerald +1;
					printf("[PID:%d] Found %d. Emerald.\n", getpid(),count_emerald);
					write(fd2[1],&value_emerald,sizeof(int));
				}

			}
        int return_value;
        read(fd1[0],&return_value, sizeof(int));
        if(return_value ==-1){
            printf("The parent will send a message to child asking her/him to exit.\n");
            exit(0);
        }
        close(fd1[0]);
		fclose(f);
		close(fd2[1]);
		exit(0);
		}


	}else{//Parent process
		close(fd2[1]);
		close(fd1[0]);
		int exit_control=0;
		int temp;
		while(temp<=200){
		read(fd2[0],&temp, sizeof(int));
		total_bucket_value = total_bucket_value + temp;
			if(total_bucket_value >200){
				close(fd2[0]);
				exit_control =-1;
				write(fd1[1],&exit_control, sizeof(int));
				close(fd1[1]);
				wait(NULL);
			}
		}
	}

	printf("Result %d\n",total_bucket_value);
	return 0;
}
