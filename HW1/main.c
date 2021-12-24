/*
 ============================================================================
 Name        : op_hw.c
 Author      : batuhan
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "functions.h"



struct PhoneBook{
	char name[50];
	char number[13];
	char *notes;

};

int main(void) {
	linkedlist *linkedlist_1 =NULL;
	char new_name[50],old_name[50],new_number[13], search_input[50], file_name[50], open_file_name[50];
	char *editInput;
	int delete_index;
	int counter=0, input=0;
	struct PhoneBook new_phoneBook;

	while(1){

				printf("1) New Entry\n");
				printf("2) List All Entries\n");
				printf("3) Edit\n");
				printf("4) Delete\n");
				printf("5) Search\n");
				printf("6) Save and Exit\n");
				printf("7) Load\n");

				scanf("%d", &input);
				if(input ==1){

					printf("Enter a name:");
					scanf("%s", new_name);
					printf("Enter a phone:");
					scanf("%s", new_number);
					printf("Enter a note:");
					scanf("\n");

					new_phoneBook.notes = input_notes();
					strcpy(new_phoneBook.name, new_name);
					strcpy(new_phoneBook.number, new_number);
					linkedlist_1 = add(linkedlist_1, new_phoneBook);
					counter++;
					printf("Entry has been successfully created. Total number of entries: %d.\n", counter);
				}else if(input ==2){
					printLinkedList(linkedlist_1);
				}else if(input ==3){
					printf("Enter a name to edit:");
					scanf("%s", old_name);
					printf("Enter a name:");
					scanf("%s", new_name);
					printf("Enter a phone:");
					scanf("%s", new_number);
					printf("Enter a note:");
					scanf("\n");

					editInput =input_notes();
					edit(linkedlist_1,old_name,new_name,new_number,editInput);

				}else if(input ==4){

					printf("Enter the index you want to delete");
					scanf("%d", &delete_index);
					delete(&linkedlist_1,delete_index);
					counter--;
					printf("Entry has been successfully deleted. Total number of entries: %d.\n", counter);
				}else if(input ==5){
					printf("Enter the name/words in notes for searching");
					scanf("%s", search_input);
					search(linkedlist_1,search_input);
				}else if(input==6){
					printf("Enter the file name that you want to save:\n");
					scanf("%s", file_name);
					save(linkedlist_1, file_name);
					printf("You successfully save and quit");
					free(linkedlist_1);
					break;
				}else if(input==7){
					printf("Enter the file name that you want to load:\n");
					scanf("%s", open_file_name);

					linkedlist_1 = load_file(linkedlist_1,open_file_name);
				}
	}

	return 0;
}