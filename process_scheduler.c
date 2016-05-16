#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <getopt.h>
#include        <assert.h>
#include        "linked_list.h"
#include        "algorithms.h"


/* Extern declarations: */
extern  char    *optarg;

/* Processes scheduler, read the processes data from a file and simulates 
   the schedule of processes acording to the selected type of algorithm
   First Come First Serve queue or Multi-level feedback queue */
int main(int argc, char** argv)
{
    char input;
    FILE *incoming_processes;
    int memsize = 200;   /* default size of memory */
    char *algorithm = "fcfs";   /* default algorithm used */
    list_t *processes_list;   

    while ((input = getopt(argc, argv, "f:a:m:")) != EOF)
    {
        
        switch ( input )
        {
            case 'f':
                /* open file if it has .txt format*/
                if(strstr(optarg, ".txt") != NULL) 
                {
                    incoming_processes = fopen(optarg, "r");

                    if(incoming_processes == NULL){
                        printf("Error opening the file, exit program\n");
                        return 1;
                    }
                    /* Make a list of all processes in file */
                    processes_list = create_process_list(incoming_processes);
                    fclose(incoming_processes);
                }
                else
                {
                    printf("Introduce a valid filename .txt\n");
                    return 1;
                }   
                break;

            case 'a':
                /* select algorithm based on optarg */
                if(strcmp(optarg, "fcfs") == 0) 
                    algorithm  = optarg;
                else if(strcmp(optarg, "multi") == 0)   
                    algorithm  = optarg;
                else                    
                {
                    printf("You need to select an algorithm either: \n \t fcfs or multi.\n");
                    return 1;
                }       
                break;
                
            case 'm':
                /* set the value of size (int value) based on optarg */
                memsize = atoi(optarg);
                if(memsize < 0)
                {
                    printf("The memory size need to be a possitive integer.\n");
                    return 1;
                }       
                break;

            default:
                break;
        } 
    }   

    /* Use the correct algorithm */
    if(strcmp(algorithm, "fcfs") == 0)
        fcfs_algoritm(processes_list, memsize);

    else if (strcmp(algorithm, "multi") == 0)
        multi_alorithm(processes_list, 2, 4, 8, memsize);

    free(processes_list);

    return 0;
}

/* Take a file.txt and put all the processes into a link list */
list_t 
*create_process_list(FILE* incoming_processes){

    int time_created;
    int process_id;
    int memsize;       
    int job_time;

    list_t* processes_list = make_empty_list(0, 0); 

    assert(incoming_processes != NULL);

    /* Read the next line and get the process data*/
    while (fscanf(incoming_processes, "%d %d %d %d", &time_created, &process_id, &memsize, &job_time) != EOF){
        insert_at_foot(processes_list, time_created, process_id, memsize, job_time, 0, 0);
    }
    return processes_list;
}
