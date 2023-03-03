/*
    Linked list
    Description: Singly-linked list that holds running (and stopped) processes
*/

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

node* head = NULL;

/*
    Function: add_to_list
    Description: Adds node to front of linked list (represents new process)
    Parameters:
        pid: Process ID (main identifier)
        path: Process path (used in print_list function)
    Return: None
*/
void add_to_list(int pid, char* path) {
    node* new_node = (node*) malloc(sizeof(node));
    new_node->pid = pid;
    new_node->path = path;
    new_node->next = head;
    head = new_node;
}

/*
    Function: remove_tail
    Description: Helper function for remove_from_list()
    Parameters: None
    Return: None
*/
void remove_tail() {
    if (head->next == NULL) {                   
        free(head);
        head = NULL;
        return;
    }
    node *curr = head;
    while (curr->next->next != NULL){                           
        curr = curr->next;
    }
    free(curr->next);                                           
    curr->next = NULL;                                          
}

/*
    Function: remove_from_list
    Description: Removes given process from anywhere in the list
    Parameters:
        pid: Process ID (main identifier)
    Return: None
*/
void remove_from_list(int remove_pid) {
    if(head == NULL){                                           // Case 1: Empty list                   
        printf("Process %d does not exist\n", remove_pid);
        return;
    }
    if(head->pid == remove_pid){                                // Case 2: Remove head        
        head = head->next;
        return;
    }
    node* curr = head;
    while(curr->next != NULL){                                  // Case 3: Remove from middle
        if(curr->next->pid == remove_pid){
            curr->next = curr->next->next;
            return;
        }
        curr = curr->next;
    }
    if(curr->pid == remove_pid){                                // Case 4: Remove tail
        remove_tail();
    }else{                                                      // Case 5: Cannot find process
        printf("Process %d does not exist\n", remove_pid);
    }
    return;
}

/*
    Function: search_for_pid
    Description: Identifies whether or not process exists (searches through list)
    Parameters:
        pid: Process ID (main identifier)
    Return: 1 if process in list, 0 otherwise
*/
int search_for_pid(int search_pid) {
    node* curr = head;
    while (curr != NULL){
        if (curr->pid == search_pid){
            return 1;                           
        }
        curr = curr->next;
    }
    return 0;                                  
}

/*
    Function: print_list
    Description: Prints information about each process in list to console
    Parameters: None
    Return: None
    Example printed process: 184028 /home/msolisdeovando/csc360/a1/hello
*/
void print_list() {
    if (head == NULL) {
        printf("No background processes\n"); 
        return;                  
    }
    node *curr = head;
    int count = 0;
    while (curr != NULL) {
        printf("%d %s\n", curr->pid, curr->path);
        count++;
        curr = curr->next;
    }
    printf("Total background processes: %d\n", count);
}