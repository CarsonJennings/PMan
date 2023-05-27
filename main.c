#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "linked_list.h"

struct Node* head = NULL;


void func_BG(char **cmd){
  if (cmd[1] == NULL) {
    return; //invalid input
  }

  pid_t pid;

  pid = fork();

  if (pid < 0) {
    perror("\nFailed to create a new process.\n");
  }

  else if (pid == 0) {
    int i = 1;
    char * argument_list[1000]; //going to need to know the number of args passed in to make the array... or just put a really big number.

    while (cmd[i] != NULL) {
      argument_list[i-1] = cmd[i];
      i++;
    }
    argument_list[i-1] = NULL;
    execvp(cmd[1], argument_list);
    exit(0);

  } else {
    head = add_newNode(head, pid, cmd[1]);
  }

}


void func_BGlist(char **cmd){
  printList(head);
}


void func_BGkill(char * str_pid){
  int pid = atoi(str_pid);
  int pid_exists = PifExist(head, pid);

  if (pid_exists == 0) {
    printf("PID: %d does not exist\n", pid);
    return;

  } else {
    head = deleteNode(head, pid);
    kill(pid, SIGTERM);
  }

}


void func_BGstop(char * str_pid){
  int pid = atoi(str_pid);
  int pid_exists = PifExist(head, pid);

  if (pid_exists == 0) {
    printf("PID: %d does not exist\n", pid);
    return;
  } else {
    kill(pid, SIGSTOP);
    printf("Stopping PID: %d\n", pid);
  }

}


void func_BGstart(char * str_pid){
  int pid = atoi(str_pid);
  int pid_exists = PifExist(head, pid);

  if (pid_exists == 0) {
    printf("PID: %d does not exist\n", pid);
    return;
  } else {
    kill(pid, SIGCONT);
    printf("Starting PID: %d\n", pid);
  }

} 


Node* clean_list(Node* node) {
  if (node == NULL){
    return node;
  }

  Node* cur = node;
  int* status = malloc(sizeof(int));

  while (cur != NULL){
    pid_t waitpid_result = waitpid(cur->pid, status, WNOHANG);

    if (waitpid_result == 0) { // no changes were made, all processes valid
      //no need to do anything, go to next itteration
    } else if (waitpid_result < 0) { // error occured 
      exit(1);
    } else {
      if (*status == WSTOPPED) { 
        // Process has been stopped no need to do anything
      } else {
        node = deleteNode(node, waitpid_result);
        printf("PID: %d has terminated\n", waitpid_result);
        cur = node;
        continue;
      }
    }

    cur = cur->next;
  }

  free(status);

  return node;
}

void pid_status(char* str_pid) {
  char filename[100];
  sprintf(filename, "/proc/%s/status", str_pid);
  char line[200];
  FILE * f = fopen(filename, "r");

  if (f == NULL) { // invalid pid error message should be printed out in pid_stat
    return;
  }

  int vol_ctxt_switches = 0;
  int non_vol_ctxt_switches = 0;

  while (fgets(line, 200, f) != NULL) {
    if (strncmp(line, "voluntary_ctxt_switches:", 24) == 0) {
      sscanf(line, "voluntary_ctxt_switches:        %d", &vol_ctxt_switches);
    } else if (strncmp(line, "nonvoluntary_ctxt_switches:", 27) == 0) {
      sscanf(line, "nonvoluntary_ctxt_switches:     %d", &non_vol_ctxt_switches);
    } else {
      continue; // Didn't find what we were looking for
    }
  }
  

  printf("voluntary_ctxt_switches:        %d\n", vol_ctxt_switches);
  printf("nonvoluntary_ctxt_switches:     %d\n", non_vol_ctxt_switches);

  fclose(f);
  return;
}

void pid_stat(char* str_pid) {
  char filename[100];
  sprintf(filename, "/proc/%s/stat" , str_pid);
  char line[1000];
  FILE* f = fopen(filename, "r");

  if (f == NULL) {
    printf("Error: Process %s does not exist\n", str_pid);
    return;
  }
  char comm[100];
  char state[2];
  long unsigned int utime;
  long unsigned int stime;
  long rss;
  int unused_int;
  unsigned unused_unsign;
  long unsigned int unused_unsign_int;
  long unused_long;
  unsigned long long unused_long_long;

  fgets(line, 1000, f);
  sscanf(line, "%d %s %s %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld", &unused_int, comm, state, &unused_int, &unused_int, &unused_int, &unused_int, &unused_int, &unused_unsign, &unused_unsign_int, &unused_unsign_int, &unused_unsign_int, &unused_unsign_int, &utime, &stime, &unused_long, &unused_long, &unused_long, &unused_long, &unused_long, &unused_long, &unused_long_long, &unused_unsign_int, &rss); //The rest that we shouldn't need %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld")

  printf("comm  -  %s\n", comm);
  printf("state -  %s\n", state);
  printf("utime -  %lu\n", utime);
  printf("stime -  %lu\n", stime);
  printf("rss   -  %ld\n", rss);

  fclose(f);
  return;
}

void func_pstat(char * str_pid){
  pid_stat(str_pid);
  pid_status(str_pid);

}


void kill_all_pid(Node* node) {
  int pid;

  while (node != NULL) {
    pid = node->pid;
    node = deleteNode(node, node->pid);
    kill(pid, SIGTERM);
  }
  return;
}

 
int main(){
    char user_input_str[50];
    while (true) {
      head = clean_list(head);
      printf("Pman: > ");
      fgets(user_input_str, 50, stdin);
      printf("User input: %s \n", user_input_str);
      char * ptr = strtok(user_input_str, " \n");
      if(ptr == NULL){
        continue;
      }
      char * lst[50];
      int index = 0;
      lst[index] = ptr;
      index++;
      while(ptr != NULL){
        ptr = strtok(NULL, " \n");
        lst[index]=ptr;
        index++;
      }
      head = clean_list(head);
      if (strcmp("bg",lst[0]) == 0){
        func_BG(lst);
      } else if (strcmp("bglist",lst[0]) == 0) {
        func_BGlist(lst);
      } else if (strcmp("bgkill",lst[0]) == 0) {
        func_BGkill(lst[1]);
      } else if (strcmp("bgstop",lst[0]) == 0) {
        func_BGstop(lst[1]);
      } else if (strcmp("bgstart",lst[0]) == 0) {
        func_BGstart(lst[1]);
      } else if (strcmp("pstat",lst[0]) == 0) {
        func_pstat(lst[1]);
      } else if (strcmp("q",lst[0]) == 0) {
        kill_all_pid(head);
        printf("Bye Bye \n");
        exit(0);
      } else {
        printf("Invalid input\n");
      }
    }

  return 0;
}

