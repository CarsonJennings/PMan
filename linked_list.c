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

 
Node * add_newNode(Node* head, pid_t new_pid, char * new_path){
	struct Node* new_node = malloc(sizeof(Node));
	new_node->pid = new_pid;
	char* path = malloc(sizeof(char) * (strnlen(new_path, 1000)+1));
	strcpy(path, new_path);
	new_node->path = path;
	new_node->next = head;

	return new_node;
}


Node * deleteNode(Node* head, pid_t pid){
	Node* cur = head;

	if (head->pid == pid) {
		head = head->next;
		free(cur->path);
		free(cur);
		return head;
	}

	

	while (cur->next != NULL) {
		if (cur->next->pid == pid) {
			Node* temp = cur->next;
			cur->next = cur->next->next;
			free(temp->path);
			free(temp);
			return head; 
		}
	}
	

	return head;
}

void printList(Node *node){
	int num_processes = 0;

	Node *cur = node;

	while (cur != NULL) {
		printf("%d:  %s \n", cur->pid, cur->path);
		num_processes++;
		cur = cur->next;
	}
	printf("Total Background Jobs: %d\n", num_processes);
	
}


int PifExist(Node *node, pid_t pid){
	while (node != NULL) {
		if (node->pid == pid) {
			return 1;
		}
		node = node->next;
	}

  	return 0;
}

