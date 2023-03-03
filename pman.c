/*
    PMan (Process Manager)
    Description: Process manager with six commands (bg, bglist, bgkill, bgstop, bgstart, pstat)
*/

#include "list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/* GLOBAL VARIABLES */
#define MAX_LINE_LEN 200
#define MAX_ARGS 100
#define PROC_FILE_LEN 2000
char* WHITESPACE = " \t\n\f\r\v";

/*
    Function: format_path
    Description: Helper function for bg(), formats path to be printed in bglist()
    Parameters:
        command: user input command (e.g. `./inf` or `ls`)
        path: process path that command will be appended to
    Return: None
*/
void format_path(char* command, char* path){
    char formatted_command[strlen(command)];
    if (command[0] == '.' && command[1] == '/') {               // Need to remove `.` from `./process`
        int i = 0;
        int j = 1;
        while (j < strlen(command)+1) {                         // Copy all contents apart from `.`
            formatted_command[i] = command[j];
            i++;
            j++;
        }
        strcpy(command, formatted_command);
    } else {
        strcat(path, "/");                                      // Process comes in from `process` instead of `./process`
    }
    strcat(path, command);                                      // Final path includes path and formatted process command
}

/*
    Function: bg
    Description: Executes new process and adds to process list
    Parameters:
        input_array: tokenized user input in string array
    Return: None
*/
void bg(char* input_array[]) {
    pid_t pid = fork();                                         // Create new child process                                
    if (pid > 0) {                                              // Parent, adds new process to list   
            char* path = getcwd(0, 0);
            format_path(input_array[1], path);
            add_to_list(pid, path);
            printf("Now executing process %d\n", pid);  
            sleep(1);                                            
    }
    else if (pid == 0) {                                        // Child, doesn't add new process to list
            execvp(input_array[1], &input_array[1]);
            printf("Error: %s is an invalid process\n", input_array[1]);
            exit(1);  
    } 
    else if (pid < 0) {                                         // Fork failed
            printf("Error: failed to fork\n");
    }
}

/*
    Function: bglist
    Description: Prints information about each process in list
    Parameters: None
    Return: None
    Example printed process: 184028 /home/msolisdeovando/csc360/a1/inf
*/
void bglist() {
    print_list();
}

/*
    Function: bgkill
    Description: Terminates given process, child_process_status removes from list
    Parameters:
        pid: Process ID of process to be terminated
    Return: None
*/
void bgkill(pid_t pid) {
    int in_list = search_for_pid(pid);
    if (in_list == 1) {
        kill(pid, SIGTERM);
    } else {
        printf("Error: Process %d does not exist\n", pid);
    }
}

/*
    Function: bgstop
    Description: Temporarily stops (pauses) given process
    Parameters:
        pid: Process ID of process to be stopped
    Return: None
*/
void bgstop(pid_t pid) {
    int in_list = search_for_pid(pid);
    if (in_list == 1) {
        kill(pid, SIGSTOP);                               
        printf("Process %d is now stopped\n", pid);
    } else {
        printf("Error: Process %d does not exist\n", pid);
    }
}

/*
    Function: bgstart
    Description: Re-starts previously stopped process
    Parameters:
        pid: Process ID of process to be started
    Return: None
*/
void bgstart(pid_t pid) {
    int in_list = search_for_pid(pid);
    if (in_list == 1) {
        kill(pid, SIGCONT);                                
        printf("Process %d has been re-started\n", pid);
    } else {
        printf("Error: Process %d does not exist\n", pid);
    }
}

/*
    Function: get_stat_array
    Description: Helper function for pstat, tokenizes proc/pid/stat contents into array
    Parameters:
        pid: Process ID to use in path
        stat_array: string array to store tokenized proc stat contents in
    Return: None
*/
void get_stat_array(pid_t pid, char* stat_array[]){
    char proc_stat_path[MAX_LINE_LEN];                          // Initialise path proc/pid/stat
    sprintf(proc_stat_path, "/proc/%d/stat", pid);
    char pid_stat_contents[PROC_FILE_LEN];                      // Initialise string to store proc stat contents in
    FILE* pointer = fopen(proc_stat_path, "r");
    if (pointer == NULL) {
        printf("Cannot read file\n");
    }
    fgets(pid_stat_contents, PROC_FILE_LEN, pointer);           // Read proc stat contents into string
    fclose(pointer);
    char* token = strtok(pid_stat_contents, WHITESPACE);        // Tokenize file contents (store in string array)
    int i = 0;
    while (i < MAX_ARGS) {                              
        stat_array[i] = token;
        token = strtok(NULL, WHITESPACE);
        i++;
    }
}

/*
    Function: get_time
    Description: Helper function for pstat, formats utime and stime
    Parameters:
        time_string: Tokenized utime/stime in stat_array
    Return: Formatted utime or stime (double)
*/
double get_time(char* time_string){
        double time;
        time = atof(time_string) / sysconf(_SC_CLK_TCK);        // Convert string to double and divide by clock ticks per second
        return time;
}

/*
    Function: get_status_contents
    Description: Helper function for pstat, reads proc/pid/status contents into string
    Parameters:
        pid: Process ID to use in path
        proc_status_contents: string to store proc status contents in
    Return: None
*/
void get_status_contents(pid_t pid, char* proc_status_contents){   
    char proc_status_path[MAX_LINE_LEN];                        // Initialise path proc/pid/status
    sprintf(proc_status_path, "/proc/%d/status", pid);  
    FILE* pointer = fopen(proc_status_path, "r");
    if (pointer == NULL) {                                      
        printf("Cannot read file\n");
    }
    fgets(proc_status_contents, PROC_FILE_LEN, pointer);        // Read all contents of proc status into string
    fclose(pointer);
}

/*
    Function: pstat
    Description: Prints information about given process
    Parameters:
        pid: Process ID of process to print information about
    Return: None
*/
void pstat(pid_t pid) {
    int in_list = search_for_pid(pid);                          
    if (in_list == 1) {
        printf("Process %d status\n", pid);
        printf("* -------------------------------- *\n");       

        char* stat_array[MAX_ARGS];                             // Tokenize proc/pid/stat contents and store in array
        get_stat_array(pid, stat_array);
        printf("comm: \t%s\n", stat_array[1]);                  // Print required info using certain indeces of stat_array
        printf("state: \t%s\n", stat_array[2]);
        printf("rss: \t%ld\n", atol(stat_array[23]));           // RSS is a long integer value
        printf("utime: \t%f\n", get_time(stat_array[13]));      // Format utime and stime
        printf("stime: \t%f\n", get_time(stat_array[14]));

        char proc_status_contents[PROC_FILE_LEN];               // Read proc/pid/status contents into string, index ctxt switches
        get_status_contents(pid, proc_status_contents);
        printf("voluntary ctxt switches: \t%d\n", proc_status_contents[39]); 
        printf("nonvoluntary ctxt switches: \t%d\n", proc_status_contents[40]);
        printf("* -------------------------------- *\n");
    } else {
        printf("Error: Process %d does not exist\n", pid);
    }
}

/*
    Function: is_valid_command
    Description: Helper function for get_user_input, 
                 determines whether or not user provided valid PMan command 
    Parameters:
        user_command: command given by user
    Return: 1 if valid command, 0
*/
int is_valid_command(char user_command[]) {
    char* valid_commands[6] = {
        "bg",
        "bglist",
        "bgkill",
        "bgstop",
        "bgstart",
        "pstat",
    };
    if (user_command == NULL) {
        return 0;
    }
    int i = 0;
    while (i < 6) {
        if (strcmp(valid_commands[i], user_command) == 0) {
            return 1;
        }
        i++;
    }
    return 0;
}

/*
    Function: execute_command
    Description: Executes PMan command specified by user
    Parameters:
        input_array: Tokenized user input
    Return: None
*/
void execute_command(char* input_array[]){
    if (strcmp(input_array[0], "bg") == 0) {               
        if (input_array[1] == NULL) {                      
            printf("Error: Process name required\n");        
        } else {                                           
            bg(input_array);
        }
    }
    if (strcmp(input_array[0], "bglist") == 0){            
        bglist();
    }
    if (strcmp(input_array[0], "bgkill") == 0) {           
        if (input_array[1] == NULL) {
            printf("Error: Process ID required\n");          
        }
        else if (atoi(input_array[1]) != 0) {
            bgkill(atoi(input_array[1]));
        } else {
            printf("Error: Process ID required\n");         
        }
    }
    if (strcmp(input_array[0], "bgstop") == 0) {           
        if (input_array[1] == NULL) {                      
            printf("Error: Process ID required\n");          
        }
        else if (atoi(input_array[1]) != 0) {             
            bgstop(atoi(input_array[1]));
        } else {                                           
            printf("Error: Process ID required\n");         
        }
    }
    if (strcmp(input_array[0], "bgstart") == 0) {          
        if (input_array[1] == NULL) {                      
            printf("Error: Process ID required\n");
        }
        else if (atoi(input_array[1]) != 0) {              
            bgstart(atoi(input_array[1]));
        } else {                                           
            printf("Error: Process ID required\n");
        }
    }
    if (strcmp(input_array[0], "pstat") == 0) {
        if (input_array[1] == NULL) {
            printf("Error: Process ID required\n");
        }
        else if (atoi(input_array[1]) != 0){
            pstat(atoi(input_array[1]));
        } else {
            printf("Error: Process ID required\n");
        }
    }
}

/*
    Function: get_user_input
    Description: Prompts user, tokenizes input, and checks for valid input
    Parameters: None
    Return: None
*/
void get_user_input(){
    char raw_input [MAX_LINE_LEN];                         
    char* input_array[MAX_ARGS];                   

    printf("PMan: > ");                                    
    fgets(raw_input, MAX_LINE_LEN, stdin);                 // Store raw user input in string

    char* token = strtok(raw_input, WHITESPACE);           // Store tokenized user input in string array
    int i = 0;
    while (i < MAX_ARGS) {                         
        input_array[i] = token;
        token = strtok(NULL, WHITESPACE);
        i++;
    }

    int valid = is_valid_command(input_array[0]);          // Check if command (first token) is valid
    if (valid == 1) {                                      
        execute_command(input_array);

    }
    else if (valid == 0 && input_array[0] != NULL){        
        printf("Error: Invalid command\n");                 
    }
}

/*
    Function: child_process_status
    Description: Continuously checks child processes for status updates
    Parameters: None
    Return: None
*/
void child_process_status(){
    int status;
    for (;;) {
        pid_t wpid = waitpid(-1, &status, WNOHANG);
        if (wpid > 0){
            if(WIFEXITED(status)) {                         // Invalid processes are exited and removed from list
                remove_from_list(wpid);
            }
            else if (WIFSIGNALED(status)) {                 // Terminated processes are removed from list
                printf("Process %d terminated\n", wpid);
                remove_from_list(wpid);
            }
        }
        else if (wpid <= 0) {
            break;
        }
    }
}

/*
    Function: main
    Description: Continuously checks child processes for status updates
                 Continuously prompts user for input
    Parameters: None
    Return: 0
*/
int main(){
    for(;;){
        child_process_status();
        get_user_input();
    }
    return 0;
}