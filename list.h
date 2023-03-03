/*
    Linked list header file
    Description: Contains linked list node structure and function prototypes
*/

#ifndef LIST
#define LIST
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/* Structure to represent a node in the linked list */
typedef struct node{
    pid_t pid;
    char* path;
    struct node* next; 
} node;

/* Function prototypes */
void add_to_list(int pid, char* path);
void remove_tail();
void remove_from_list(int remove_pid);
int search_for_pid(int search_pid);
void print_list();

#endif /* LIST */
