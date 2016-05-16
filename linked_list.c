#include        <stdio.h>
#include        <stdlib.h>
#include        <assert.h>
#include 		"linked_list.h"

/* Code modified based on Alistair Moffat listops.c, discalimer at the bottom */

/* Linked list of processes operations */
/*Let you manipulate and use a linked list of processes */

/* Create an empty list */
list_t
*make_empty_list(int quantum, int list_id) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	list->quantum = quantum;
	list->list_id = list_id;
	return list;
}

/* Check if ther is no item in the list */
int
is_empty_list(list_t *list) {
	assert(list!=NULL);
	return list->head==NULL;
}

/* Free all processes in the queue */
void
free_list(list_t *list) {
	process_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
}

/* Insert a process at the head of the list*/
list_t
*insert_at_head(list_t *list, int time_created, int process_id, int memsize, int job_time, int list_id, int queue_quantum){

	process_t *new;
	new = (process_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	/* Fill the process data */
	new->time_created = time_created;
	new->process_id = process_id;
	new->memsize = memsize;	
	new->job_time = job_time;
	new->quantum = 0; /* Quantum always 0 when adding to the queue */
	new->queue_quantum = queue_quantum;
	new->list_id = list_id;

	/* Change the previous process of the current head*/
    if(list->head != NULL){
        list->head->previous = new;
    }

	new->next = list->head;
	new->previous = NULL;	

	list->head = new;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->foot = new;
	}
	return list;
}

/*Insert process at foot*/
list_t
*insert_at_foot(list_t *list, int time_created, int process_id, int memsize, int job_time, int list_id, int queue_quantum){
	
	process_t *new;
	new = (process_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	/* Fill the process data */
	new->time_created = time_created;
	new->process_id = process_id;
	new->memsize = memsize;	
	new->job_time = job_time;
	new->quantum = 0; /* Quantum always 0 when adding to the queue */
	new->queue_quantum = queue_quantum;
	new->list_id = list_id;

	new->next = NULL;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		new->previous = NULL;
		list->head = list->foot = new;
	} else {		
		new->previous = list->foot;
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

/* Check the process at the head */
process_t
*get_head(list_t *list){
	assert(list!=NULL && list->head!=NULL);
	return list->head;
}

/* Remove the process at the head from the list */
process_t 
*remove_head(list_t *list){
	assert(list!=NULL && list->head!=NULL);
	process_t *curr;
	curr = list->head;
	list->head = curr->next;
	if(list->head != NULL)
		list->head->previous= NULL;	
	/* The list is empty */
	else
		list->foot = NULL;
	return curr;
	
}

/* Remove the process at the head from the list */
process_t 
*remove_foot(list_t *list){
	assert(list!=NULL && list->foot!=NULL);
	process_t *curr;
	curr = list->foot;
	list->foot = curr->previous;
	if(list->foot != NULL)
		list->foot->next= NULL;	
	/* The list is empty */
	else
		list->head = NULL;
	return curr;

}

/* Look if the process is in the list */
int
process_in_list(list_t *list, process_t *process){
	assert(list != NULL && process != NULL);
	return (list->list_id == process->list_id);
}

/* Print statemets for debugging */
void
print_process(process_t * process){
	assert(process != NULL);
	printf("process_id = %d\t  memsize = %d\t  time_created =%d\t  job_time = %d\t quantum = %d\n", 
		process->process_id, process->memsize, process->time_created, process->job_time, process->quantum);
}

/*Print the entire list*/
void
print_list(list_t *list){
    process_t *curr;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		print_process(curr);		
		curr = curr->next;
	}
	printf("<--------------------------------------------------------------------->\n");
}

/* =====================================================================
   Program based on listops.c written by Alistair Moffat, as an example 
   for the book "Programming, Problem Solving, and Abstraction with C", 
   Pearson Custom Books, Sydney, Australia, 2002; revised edition 2012,
   ISBN 9781486010974.

   See http://people.eng.unimelb.edu.au/ammoffat/ppsaa/ for further
   information.

   Prepared December 2012 for the Revised Edition.
   ================================================================== */