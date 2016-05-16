#include        <stdio.h>
#include        <stdlib.h>
#include        <assert.h>
#include        "memory.h"

#define TOP_ADRESS 1 /* First address after in acending order */

/* Code modified based on Alistair Moffat listops.c, discalimer at the bottom */

/* Linked list of processes images in memory */
/* Let you manipulate and use memory in form of a liked list*/

/* Create an empty memory */
memory_t
*make_empty_memory(int size) {
    memory_t *memory;
    memory = (memory_t*)malloc(sizeof(*memory));
    assert(memory!=NULL);
    memory->head = memory->foot = NULL;
    memory->size = size;
    memory->num_process = 0;
    memory->used_memory = 0;
    return memory;
}

/* Check if ther is no item in the memory */
int
is_empty_memory(memory_t *memory) {
    assert(memory!=NULL);
    return memory->head==NULL;
}

/* Free all processes in the queue */
void
free_memory_list(memory_t *memory) {
    m_process_t *curr, *prev;
    assert(memory!=NULL);
    curr = memory->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(memory);
}

/* Find the closest memory hole to the top adress that the process fits into, and insert it.
   If there is not a hole large enough it, do a swap with the largest process in memory, 
   take the oldest when to large process have the same size */
memory_t
*insert_process(memory_t *memory, int time_allocated, int process_id, int memsize){
    
    assert(memory !=NULL && memory->size >= memsize);

    /* Runs until the process find a hole to fit,  */
    while(1){ 
        /* It is the first process in memory */
        if (is_empty_memory(memory)){     
                   
            return insert_at_head_m(memory, time_allocated, process_id, memsize);
        }    

        m_process_t *curr;
        curr = memory->head;
        m_process_t *largest;    

        int hole_size;                             
        
        /* Check for a hole to fit */
        while (curr) {  

            /* Current at head */
            if(curr->previous == NULL){  

                
                hole_size = curr->relative_pos - TOP_ADRESS;
                
                if(hole_size >= memsize){                    

                    return insert_at_head_m(memory, time_allocated, process_id, memsize);   
                }

                /* Set the first process as the largest */
                largest = curr;
                
                /* Check if there is only one process in memory */
                if (curr->next == NULL){
                    
                    hole_size = memory->size - curr->relative_end + 1;
                    
                    if(hole_size >= memsize){
                        return insert_at_foot_m(memory, time_allocated, process_id, memsize);                 
                    } else {
                        memory = remove_process(memory, largest);                                                                             
                    }          
                }
               
            /* Current at foot */
            } else if (curr->next == NULL){
                /* The hole at foot distance between the last process address and the end of the memory*/
                hole_size = memory->size - curr->relative_end + 1;
                if(hole_size >= memsize){
                    return insert_at_foot_m(memory, time_allocated, process_id, memsize);                 
                } else {                                  
                    compare_size(curr, largest);                                         

                    /* Swap the process with the largest one in memory */
                    memory = remove_process(memory, largest);                                                    
                }
            } else {            
                hole_size = curr->relative_pos - curr->previous->relative_end + 1;
                if(hole_size >= memsize){
                    return insert_before(memory, curr, time_allocated, process_id, memsize);                    
                }
                /* Check if the current process is the largest, if another process has equal size he oldest is returned */
                compare_size(curr, largest);
            }
            curr = curr->next;
        }        
    }
    return memory;
}

/* Return the number of holes in memory*/
int 
num_holes(memory_t *memory){
    assert(memory != NULL);
    int num = 0;
    /* One hole, the entire memory */
    if(is_empty_memory(memory)){
        return 1;
    }

    m_process_t *curr;
    curr = memory->head;

    while(curr){ 
        if(curr->previous == NULL){
            if(curr->relative_pos != TOP_ADRESS){
                num += 1;
            }
        }
        if (curr->next == NULL)
        {
            if(curr->relative_end != memory->size){
                num += 1;
            }
        } else {
            if((curr->relative_end + 1) != curr->next->relative_pos){
                num += 1;
            }
        }
        curr = curr->next;
    }
    curr = NULL;
    free(curr);
    return num;
}


/* Insert a process before a process*/
memory_t 
*insert_before(memory_t *memory, m_process_t *process, int time_allocated, int process_id, int memsize){
   
    assert(memory->size >= memsize);
    m_process_t *new;
    new = (m_process_t*)malloc(sizeof(*new));
    assert(memory!=NULL && new!=NULL);

    new->time_allocated = time_allocated;
    new->process_id = process_id;
    new->memsize = memsize; 
    new->relative_pos = process->previous->relative_end + 1;
    new->relative_end = new->relative_pos + memsize - 1;

    new->next = process;
    if(process != NULL){
        if(process->previous == NULL){
            memory->head = new;
        }
        process->previous = new;
    }
    new->previous = process->previous;
    /* New process is at the head */ 
    memory->used_memory += memsize;
    memory->num_process += 1;
    return memory;
}
/* Insert a process at the head of the memory*/
memory_t 
*insert_at_head_m(memory_t *memory, int time_allocated, int process_id, int memsize){

    assert(memory->size >= memsize);
    m_process_t *new;
    new = (m_process_t*)malloc(sizeof(*new));
    assert(memory!=NULL && new!=NULL);
    /* Fill the process data */
    new->time_allocated = time_allocated;
    new->process_id = process_id;
    new->memsize = memsize; 
    new->relative_pos = TOP_ADRESS;
    new->relative_end = memsize;

    new->previous = NULL;
    new->next = memory->head;
    if(memory->head != NULL){
        memory->head->previous = new;
    } else {
       /* this is the first insertion into the memory */
       memory->foot = new;
    }     
    memory->head = new;
    memory->used_memory += memsize;
    memory->num_process += 1;
    return memory;
}

/*Insert process at the foot of the memory */
memory_t 
*insert_at_foot_m(memory_t *memory, int time_allocated, int process_id, int memsize){
    
    assert(memory->size >= memsize);   
    m_process_t *new;
    new = (m_process_t*)malloc(sizeof(*new));
    assert(memory!=NULL && new!=NULL);
    /* Fill the process data */
    new->time_allocated = time_allocated;
    new->process_id = process_id;
    new->memsize = memsize; 

    new->next = NULL;

    if (memory->foot==NULL) {
        /* this is the first insertion into the memory */
        new->relative_pos = TOP_ADRESS;
        new->relative_end = memsize;
        memory->head = memory->foot = new;
    } else { 
        new->relative_pos = memory->foot->relative_end + 1;
        new->relative_end = new->relative_pos + memsize - 1;        
        memory->foot->next = new;
        memory->foot = new;
    }

    new->previous = memory->foot;    
    memory->used_memory += memsize;

    memory->num_process += 1;
    return memory;
}

/* Remove a process from memory assuming the process is already there
   IMPORTANT: Make sure the process is in memory */
memory_t 
*remove_process(memory_t *memory, m_process_t *process){
    
    assert(memory!=NULL && process!=NULL);
    m_process_t *curr;
    curr = process;    
           
    if(curr->next == NULL){
        memory->foot = curr->previous; 
        if(curr->previous != NULL)
            curr->previous->next = NULL;
    }
    if(curr->previous == NULL){     
        memory->head = curr->next;
        if(curr->next != NULL)
            curr->next->previous = NULL;
    }

    if(curr->previous != NULL && curr->previous->next != NULL)
        curr->previous->next = curr->next;
    if(curr->next != NULL && curr->next->previous != NULL)   
        curr->next->previous = curr->previous;
    
    memory->used_memory -= process->memsize;
    memory->num_process -= 1;
    return memory;
}

/* Remove a process based on its id */
memory_t 
*remove_process_id(memory_t *memory, int process_id){

    assert(memory!=NULL);    
    m_process_t *curr;
    curr = memory->head;
    /* Search for process*/
    while(curr){

        if(curr->process_id == process_id){
            
            if(curr->next == NULL){
                memory->foot = curr->previous; 
                if(memory->foot != NULL){
                    memory->foot->next = NULL;
                }
            }

            if(curr->previous == NULL){     
                memory->head = curr->next;
                if(memory->head != NULL)
                    memory->head->previous = NULL;
            }
            /* Connect the processes*/
            if(curr->previous != NULL && curr->previous->next != NULL)
                curr->previous->next = curr->next;
            if(curr->next != NULL && curr->next->previous != NULL)   
                curr->next->previous = curr->previous;
            break;
        }

        curr = curr->next;
    }
    memory->used_memory -= curr->memsize;
    memory->num_process -= 1;
    return memory;
}

/* Chec if the process is in memory*/
int
check_process_id(memory_t *memory, int process_id){

    assert(memory!=NULL);
    m_process_t *curr;
    curr = memory->head;

    while(curr){
        if(curr->process_id == process_id){
            return 1;
        }
        curr = curr->next;
    }    
    return 0;
}

/*Compare the and return the largest process between two*/
void
compare_size(m_process_t *curr, m_process_t *largest){

    assert(curr!=NULL && largest!=NULL);

    if(curr->memsize == largest->memsize){
        if(curr->time_allocated < largest->time_allocated){
           largest = curr; 
        }
    }
      
    if(curr->memsize >= largest->memsize){
        largest = curr;        
    }   

}

/* Print statemets for debugging */
void
print_m_process(m_process_t * process){
    assert(process != NULL);
    printf("process_id = %d\t  memsize = %d\t  time_allocated =%d\n", 
        process->process_id, process->memsize, process->time_allocated);
}

/*Print the entire memory*/
void
print_memory(memory_t *memory){
    m_process_t *curr;
    assert(memory!=NULL);
    printf("<--------------------------------------------------------------------->\n");
    curr = memory->head;
    while (curr) {
        print_m_process(curr);        
        curr = curr->next;
    }
    curr = NULL;
    free(curr);
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