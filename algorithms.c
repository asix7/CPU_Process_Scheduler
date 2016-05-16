/*First Come First Serve and Multi-level feedback queues algorithms for processes schedule*/
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <assert.h>
#include        "linked_list.h"
#include        "algorithms.h"



/* Implementation of First Come First Serve Queue Algorithm. Put each incoming process in a queue, 
   allocate them in memory and give the first process in the queue the usage of the CPU */ 
void
fcfs_algoritm(list_t *processes_list, int size_of_memory){

    int clock_time = 0; /* Time from the start of program */
    process_t *running = NULL; /* Current process running in the CPU*/
    process_t *finished_process;
    list_t *queue = make_empty_list(0, 1);
    memory_t *memory = make_empty_memory(size_of_memory);

    process_to_queue(processes_list, queue, clock_time);

    /* Don't stop before all processes have been handled */
    while(!is_empty_list(processes_list) || !is_empty_list(queue) || running != NULL){

        process_to_queue(processes_list, queue, clock_time);

        /* Run the process at the top of the queue */
        if(running == NULL || running->job_time == 0){

            
            if(running != NULL) {
                if(running->job_time == 0) {
                    remove_process_id(memory, running->process_id);
                }
            }

            if(!is_empty_list(queue)){

                running = get_head(queue);
                insert_process(memory, clock_time, running->process_id, running->memsize);                            
                printf("time %d, %d running, numprocesses=%d, numholes=%d, memusage=%d%%\n", 
                       clock_time, running->process_id, memory->num_process, num_holes(memory), (int)(100*memory->used_memory/(double)size_of_memory));
                remove_head(queue);
            } else {
                finished_process = running;
                free(finished_process);            
                running = NULL; 
            }   
        }

        /* Decrease the time of the running process */
        if(running != NULL){
            running->job_time -= 1;            
        }

        clock_time += 1;            
    }      

    /* Print at the end of simulation, take one second of the last addition */
    printf("time %d, simulation finished.\n", clock_time - 1);
    free(running);
    free_list(queue);
}

/* Implementation of Multi-level feedback queue Algorithm. Put each incoming process in a queue.
   Select the running process according 3 priority level queues when running process has exhausted all its 
   quantum or completed its job. 
    */ 
void 
multi_alorithm(list_t *processes_list, int first_quantum, int second_quantum, int third_quantum, int size_of_memory){

    int clock_time = 0; /* Time from the start of program */
    process_t *running = NULL; /* Current process running in the CPU */

    memory_t *memory = make_empty_memory(size_of_memory);
        
    /* Make all the queues levels */
    list_t *queue_1 = make_empty_list(first_quantum, 1);
    list_t *queue_2 = make_empty_list(second_quantum, 2);
    list_t *queue_3 = make_empty_list(third_quantum, 3);

    process_to_queue(processes_list, queue_1, clock_time);

    /* Don't stop before all processes have been handled */
    while(!is_empty_list(processes_list) || !is_empty_list(queue_1) || 
          !is_empty_list(queue_2) || !is_empty_list(queue_3) || running != NULL){


        process_to_queue(processes_list, queue_1, clock_time);

        if(running == NULL || running->quantum == running->queue_quantum) {

            running = select_running(queue_1, queue_2, queue_3);


            if(running != NULL){

                /* Put the process in memory if is not*/
                if(!check_process_id(memory, running->process_id)){                    
                     
                    memory = insert_process(memory, clock_time, running->process_id, running->memsize); 

                }

                printf("time %d, %d running, numprocesses=%d, numholes=%d, memusage=%d%%\n", 
                    clock_time, running->process_id, memory->num_process, num_holes(memory), (int)(100*memory->used_memory/(double)size_of_memory)); 

            }
                       
        }      
            
        if(running != NULL) {  
            /* Update the process */
            running->job_time -= 1;
            running->quantum += 1; 


            if (process_in_list(queue_1, running)){
                running = check_process(running, queue_1, queue_2, memory);
            }

            else if(process_in_list(queue_2, running)){ 
                running = check_process(running, queue_2, queue_3, memory);
            }

            else if(process_in_list(queue_3, running)){
                running = check_process(running, queue_3, queue_3, memory);
            }             

        }   

        clock_time += 1;
    }

    printf("time %d, simulation finished.\n", clock_time);
    free(running);
    free_list(queue_1);
    free_list(queue_2);
    free_list(queue_3);
}


/* Select next process to run in the CPU*/
process_t 
*select_running(list_t *queue_1, list_t *queue_2, list_t *queue_3){


    /* Select the process with highest priority of all three levels*/
    if(is_empty_list(queue_1)){
        if(is_empty_list(queue_2)){
            if(!is_empty_list(queue_3)){
                return get_head(queue_3);
            }
        } else {
            return get_head(queue_2);         
        }
    } else {
        return get_head(queue_1);
    }
    return NULL;    
}

/* Swap the process to the next queue when its quantum is exahusted */
void
swap_queue(list_t *curr_queue, list_t *next_queue){

    assert(!is_empty_list(curr_queue));

    process_t *process_holder = get_head(curr_queue);

    if(process_holder->quantum == curr_queue->quantum){        
        remove_head(curr_queue);
        insert_at_foot(next_queue, process_holder->time_created, process_holder->process_id, 
                       process_holder->memsize, process_holder->job_time, next_queue->list_id, next_queue->quantum);
    }
}  

/* Put the process that has exhausted its quantum to the back of the queue */
void
round_robin(list_t *queue){

    assert(!is_empty_list(queue));

    /* RR for the last queue */
    process_t *process_holder = get_head(queue);    

    if(process_holder->quantum == queue->quantum){
        remove_head(queue);
        queue = insert_at_foot(queue, process_holder->time_created, process_holder->process_id, 
                       process_holder->memsize, process_holder->job_time, queue->list_id, queue->quantum);        
    }         
}

/* Determines what to do if the process exhausted its quantum or finished the job */
process_t
*check_process(process_t *running, list_t *curr_queue, list_t *next_queue, memory_t *memory){
    /*Check if the process need to be swaped or put at the end of the list*/
    if(curr_queue->list_id != next_queue->list_id)
        swap_queue(curr_queue, next_queue);
    else 
        round_robin(curr_queue);

     /* Check for finished job and remove the process */
    if(running->job_time == 0){
        /* The process will be at the head of the current queue or at the end of the next queue
           depending if it has being swapped around*/
        
        memory = remove_process_id(memory, running->process_id);   

        if(!is_empty_list(curr_queue) && get_head(curr_queue)->process_id == running->process_id)
            free(remove_head(curr_queue));
        else
            free(remove_foot(next_queue));
        running = NULL; 
    }
    return running;
}

/* Check the next process in the list and if it is created at the time put it into a queue */
void
process_to_queue(list_t *processes_list, list_t *queue, int clock_time){

    process_t *new_come;
    process_t *process_to_free;

    /* Put the first process in the queue and remove from list*/
    if(clock_time == 0 && is_empty_list(queue)) {

        assert(!is_empty_list(processes_list));
        
        new_come = get_head(processes_list);

        assert(new_come->time_created == 0);

        insert_at_foot(queue, new_come->time_created, new_come->process_id, 
                       new_come->memsize, new_come->job_time, queue->list_id, queue->quantum);
        process_to_free = remove_head(processes_list);
        free(process_to_free);

    /* Put the processes in a queue at the time they come */    
    } else {    
        
        if(!is_empty_list(processes_list)){  

            new_come = get_head(processes_list);
            while(new_come->time_created == clock_time){
                
                insert_at_foot(queue, new_come->time_created, new_come->process_id, 
                               new_come->memsize, new_come->job_time, queue->list_id, queue->quantum);
                process_to_free = remove_head(processes_list);
                free(process_to_free);
                
                if(!is_empty_list(processes_list))     
                    new_come = get_head(processes_list); 
                else
                    break;             
            }      
        }       
    } 
}