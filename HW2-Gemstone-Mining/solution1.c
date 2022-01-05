/*
 ============================================================================
 Name        : solution1.c
 Author      : 
 Version     ://Reading bucket values from child using pipes
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
	int counter,lower,upper,bucket_value;

	counter =0;// 24 char counter
	bucket_value =0; // Total value of bucket value
	lower = 500; // Least value for msec.
	upper = 800; // Max value for msec.

	//Count for mines
	int count_diamond =0;
	int count_ruby =0;
	int count_sapphire =0;
	int count_emerald =0;


	// Define pipes
	int fd[2];
	pipe(fd);

	char file_name[13] ="buckets/0.txt"; //File name
	int total_bucket_value =0; //Total value of bucket

	if(pipe(fd) ==-1){// Controlling pipe
		printf("An error occurred with opening the pipe\n");
	}
	int temp;// variable for getting bucket values while reading from pipe
	int i;// variable for number of forks
	for(i=0;i<5;i++){
		// Changing index of file name with real index
		file_name[8] = i +'0';


		// Creating forks
		int id = fork();
		if(id ==0){
			printf("[PID:%d] A new child has been created with PID:%d\n", getppid(), getpid());

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
				}
				else if(ch == 'r'){
					count_ruby = count_ruby +1;
					printf("[PID:%d] Found %d. Ruby.\n", getpid(),count_ruby);
				}
				else if(ch == 's'){
					count_sapphire = count_sapphire +1;
					printf("[PID:%d] Found %d. Sapphire.\n", getpid(),count_sapphire);
				}
				else if(ch == 'e'){
					count_emerald = count_emerald +1;
					printf("[PID:%d] Found %d. Emerald.\n", getpid(),count_emerald);
				}
			}
			//Writing values to pipe
			fclose(f);
			bucket_value = (count_diamond*3500)+(count_ruby*50)+(count_sapphire*1200)+(count_emerald*800);
			close(fd[0]);
			write(fd[1],&bucket_value, sizeof(int));
			close(fd[1]);
			exit(0);
		}

	}

	//Reading bucket values from child using pipes
	close(fd[1]);
	int k;
	for(k=0;k<5;k++){
	read(fd[0],&temp, sizeof(int));
	total_bucket_value = total_bucket_value + temp;
	}
	close(fd[0]);

	printf("[PID:%d] Result %d\n", getppid(),total_bucket_value);

	//Wait child process
	for(int i=0;i<5;i++){
	    wait(NULL);
	}

	return 0;
}
