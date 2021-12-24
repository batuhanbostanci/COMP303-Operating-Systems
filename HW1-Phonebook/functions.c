/*
 * functions.c
 *
 *  Created on: Nov 6, 2021
 *      Author: batuhan
 */

#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

struct PhoneBook{
	char name[50];
	char number[13];
	char *notes;

};

 struct LinkedList{
	struct PhoneBook phonebook;
	struct LinkedList *linkedlist;
};

typedef struct PhoneBook phonebook;
typedef struct LinkedList linkedlist;



void printLinkedList(linkedlist *list){
	linkedlist *temp = list;

	if(temp ==NULL){
		printf("The list is empty\n");
	}else{
		while(temp !=NULL){
			printf("%s\n", temp->phonebook.name);
			printf("%s\n", temp->phonebook.number);
			printf("%s\n", temp->phonebook.notes);

			temp = temp->linkedlist;

		}
	}
	free(temp);

}

linkedlist *add(linkedlist *list, phonebook new_phonebook){
	linkedlist *temp1;
	temp1 = (linkedlist*)malloc(sizeof(linkedlist));

	temp1->phonebook = new_phonebook;
	temp1->linkedlist = NULL;


	if(list == NULL || strcmp(temp1->phonebook.name, list->phonebook.name)<0){
		temp1->linkedlist=list;
		return temp1;
	}else{
		linkedlist *temp2 = list;
		while(temp2->linkedlist !=NULL && strcmp(temp1->phonebook.name,temp2->phonebook.name)>=0){
			temp2 = temp2->linkedlist;
		}
		temp1->linkedlist=temp2->linkedlist;
		temp2->linkedlist = temp1;

		return list;
	}

}

void edit(linkedlist *list, char oldName[], char newName[], char newNumber[], char *newNote){
	linkedlist *tempList =list;
	while(tempList !=NULL){
		if(strcasecmp(tempList->phonebook.name,oldName)==0){
			strcpy(tempList->phonebook.name,newName);
			strcpy(tempList->phonebook.number,newNumber);
			tempList->phonebook.notes=newNote;

			printf("Editing is done!\n");
			return;
		}else
			tempList = tempList->linkedlist;

	}
	printf("There is no name in phonebook. Please try again!\n");
}

char* input_notes(){
	char c;
	int i=0;
	char *note_value=NULL;

	note_value = (char*)malloc(1*sizeof(char));

	while(c !='.'){
		c = getc(stdin);
		note_value[i]=c;
		i= i+1;
		note_value = (char*)realloc(note_value,(i+1)*sizeof(char));
	}

	note_value[i]='\0';

	return note_value;
}
void search(linkedlist *list, char search_input[]){
	linkedlist *tempList = list;
	while(list !=NULL){
		if(strcasecmp(tempList->phonebook.name,search_input)==0 || strstr(tempList->phonebook.notes,search_input)!=0){
		 printf("Search is successfully done\n");
		 printf("Name:%s\n", tempList->phonebook.name);
		 printf("Number:%s\n", tempList->phonebook.number);
		 printf("Notes:%s\n", tempList->phonebook.notes);
		 return;
		}else
		tempList = tempList->linkedlist;

	}
	free(tempList);
	printf("The word is not in the phonebook:(\n");
}
void save(linkedlist *list, char name[]){
	FILE *fptr;
	linkedlist *temp=list;
	fptr = fopen(strcat(name,".txt"),"w");

	if(fptr ==NULL){
		printf("Error was occurred\n");
		exit(1);
	}

	while(temp !=NULL){
		fprintf(fptr,"%s\n%s\n%s\n", temp->phonebook.name,temp->phonebook.number,temp->phonebook.notes);
		temp = temp->linkedlist;
	}
	fclose(fptr);
	free(temp);

}
linkedlist *load_file(linkedlist *list, char name[]){
	linkedlist *temp_list=list;
	FILE *fptr;
	char *temp= malloc(1000);
	int count =0;
	phonebook phone;

	fptr = fopen(strcat(name,".txt"),"r");

	if(fptr == NULL){
		printf("Error was occurred\n");
		exit(1);
	}
	while(fgets (temp, 1000, fptr)!=NULL){

		if(count==0){
			int length=strlen(temp);
			temp[strlen(temp)-1] = '\0';
			strcpy(phone.name,temp);

		count++;
		}
		else if(count ==1){
			int length=strlen(temp);

			temp[strlen(temp)-1] = '\0';
			strcpy(phone.number,temp);
		count++;
		}
		else if(count ==2){
			int length=strlen(temp);
			temp[strlen(temp)-1] = '\0';
			strcpy(phone.notes,temp);
		count++;
		}
		if(count ==3){
			count =0;
			temp_list = add(temp_list,phone);
		}
	}
	free(temp);
	fclose(fptr);
	return temp_list;


}

void delete(linkedlist **list, int index){
	linkedlist *temp=*list;
	linkedlist *temp2;
	char answer;
	int temp_counter=0;

	if(index==1){
		printf("Entry '%s' will be deleted. Are you sure? (y/n)\n",temp->phonebook.name);
		scanf(" %c", &answer);
		if(answer=='y'||answer=='Y'){

		*list=temp->linkedlist;
		free(temp);
		return;
		}else if(answer =='n'||answer=='N'){
			return;
		}
	}

	while(temp_counter < index-2){
		temp= temp->linkedlist;
		temp_counter++;
	}
	temp2= temp->linkedlist;
		printf("Entry '%s' will be deleted. Are you sure? (y/n)\n",temp2->phonebook.name);
		scanf(" %c", &answer);
		if(answer=='y'||answer=='Y'){
		temp->linkedlist = temp2->linkedlist;
		free(temp2);
		return;
		}else if(answer =='n'||answer=='N'){
			return;
		}

}