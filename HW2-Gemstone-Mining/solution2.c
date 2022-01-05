/*
 ============================================================================
 Name        : solution2.c
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





//Structure of massage passing
struct mesg_buffer {
    long mesg_type;
    int bucket_value;
} message;


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

	//Massage passing variables
	key_t key;
	int msgid;

	// ftok to generate unique key
	key = ftok("progfile", 65);

	msgid = msgget(key, 0666 | IPC_CREAT);
	message.mesg_type = 1;

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
					message.bucket_value=3500;
					msgsnd(msgid,&message, sizeof(message),0);

				}
				else if(ch == 'r'){
					count_ruby = count_ruby +1;
					printf("[PID:%d] Found %d. Ruby.\n", getpid(),count_ruby);
					message.bucket_value=50;
					msgsnd(msgid,&message, sizeof(message),0);

				}
				else if(ch == 's'){
					count_sapphire = count_sapphire +1;
					printf("[PID:%d] Found %d. Sapphire.\n", getpid(),count_sapphire);
					message.bucket_value=1200;
					msgsnd(msgid,&message, sizeof(message),0);

				}
				else if(ch == 'e'){
					count_emerald = count_emerald +1;
					printf("[PID:%d] Found %d. Emerald.\n", getpid(),count_emerald);
					message.bucket_value=800;
					msgsnd(msgid,&message, sizeof(message),0);
				}
			}
			message.bucket_value=-1;
			msgsnd(msgid,&message, sizeof(message),0);
			fclose(f);
			exit(0);

		}

	}


	int temp_counter=0;//for counting -1 to control message queues
	while(1){
		msgrcv(msgid, &message, sizeof(message), 1, 0);

		if(message.bucket_value==-1){//To control, end of the bucket file
			temp_counter++;
		}else{
		total_bucket_value = total_bucket_value + message.bucket_value;//addition of bucket values
		}

		if(temp_counter==i)//If it is equal to number of txt files. Then break
			break;
	}
	msgctl(msgid, IPC_RMID, NULL);

	//Wait child process
	for(int i=0;i<5;i++){
	    wait(NULL);
	}
	printf("[PID:%d] Result %d\n", getppid(),total_bucket_value);

	return 0;
}