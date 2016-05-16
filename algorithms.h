#include        "memory.h"

/*Header for Algorithms*/

list_t *create_process_list(FILE* incoming_processes);

void fcfs_algoritm(list_t *processes_list, int size_of_memory);

void multi_alorithm(list_t *processes_list, int first_quantum, int second_quantum, int third_quantum, int size_of_memory);

process_t *select_running(list_t *queue_1, list_t *queue_2, list_t *queue_3);

void round_robin(list_t *queue);

void swap_queue(list_t *curr_queue, list_t *next_queue);

process_t *check_process(process_t *running, list_t *curr_queue, list_t *next_queue, memory_t *memory);

void process_to_queue(list_t *processes_list, list_t *queue, int clock_time);