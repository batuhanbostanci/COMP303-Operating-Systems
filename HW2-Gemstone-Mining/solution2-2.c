/*
 ============================================================================
 Name        : 2.c
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
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/msg.h>



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

	char file_name[13] ="buckets/0.txt"; //File name
	int total_bucket_value =0; //Total value of bucket



	int i;// variable for number of forks
	for(i=0;i<5;i++){
		// Changing index of file name with real index
		file_name[8] = i +'0';



		// Creating forks
		int id = fork();
		if(id ==0){

			// ftok to generate unique key
			key_t key = getpid();
			// shmget returns an identifier in shmid
			int shmid = shmget(key,1024,0666|IPC_CREAT);
			// shmat to attach to shared memory
			char *str =  shmat(shmid,0,0);

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
					char ch='d';
					strncat(str, &ch,1);


				}
				else if(ch == 'r'){
					count_ruby = count_ruby +1;
					printf("[PID:%d] Found %d. Ruby.\n", getpid(),count_ruby);
					char ch='r';
					strncat(str, &ch,1);

				}
				else if(ch == 's'){
					count_sapphire = count_sapphire +1;
					printf("[PID:%d] Found %d. Sapphire.\n", getpid(),count_sapphire);
					char ch='s';
					strncat(str, &ch,1);

				}
				else if(ch == 'e'){
					count_emerald = count_emerald +1;
					printf("[PID:%d] Found %d. Emerald.\n", getpid(),count_emerald);
					char ch='e';
					strncat(str, &ch,1);

				}


			}
			shmdt(str);//Kill shmdt
			fclose(f);
			exit(0);

		}
	}


	int ptr;//Variable for id of the the message

	//Wait child process and summation of buckets
	int k;
	for(k=0;k<5;k++){
		pid_t procces_id = wait(NULL);
		key_t new_key =procces_id;
		ptr = shmget(new_key,1024,0666|IPC_CREAT);
		char *newchar = shmat(ptr, 0,0);
		int t;//Variable for index of string
		int bucket_value;//Variable for mine value
		for(t=0;t<strlen(newchar);t++){
			if(newchar[t]=='d')
				bucket_value =3500;
			else if(newchar[t]=='r')
				bucket_value =50;
			else if(newchar[t]=='s')
				bucket_value =1200;
			else if(newchar[t]=='e')
				bucket_value =800;

			total_bucket_value = total_bucket_value + bucket_value;
		}
		shmdt(newchar);
	}
	shmctl(ptr,IPC_RMID,NULL);


	printf("[PID:%d] Result %d\n", getppid(),total_bucket_value);

	return 0;
}
