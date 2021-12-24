/*
 * functions.h
 *
 *  Created on: Nov 6, 2021
 *      Author: batuhan
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


typedef struct LinkedList linkedlist;
typedef struct PhoneBook phonebook;

void printLinkedList(linkedlist *list);
linkedlist *add(linkedlist *list, phonebook new_phonebook);
void edit(linkedlist *list, char oldName[], char newName[],char newNumber[],char *newNote);
char* input_notes();
void search(linkedlist *list, char search_input[]);
void save(linkedlist *list, char name[]);
linkedlist *load_file(linkedlist *list,char name[]);
void delete(linkedlist **list, int index);

#endif /* FUNCTIONS_H_ */