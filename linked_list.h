/* Linked list of processes operations */

typedef struct process process_t;

struct process {
    /* Process data */
    int time_created;
    int process_id;
    int memsize; 
    int job_time;
    int quantum;
    /* Process' list datac*/
    int list_id;
    int queue_quantum;
    
    process_t *previous;
    process_t *next;

};

typedef struct {
    process_t *head;
    process_t *foot;
    int quantum; 
    int list_id;   
} list_t;



list_t *make_empty_list(int quantum, int list_id);

int is_empty_list(list_t *list);

void free_list(list_t *list);

list_t *insert_at_head(list_t *list, int time_created, int process_id, int memsize, int job_time, int list_id, int queue_quantum);

list_t *insert_at_foot(list_t *list, int time_created, int process_id, int memsize, int job_time, int list_id, int queue_quantum);

process_t *get_head(list_t *list);

process_t *remove_head(list_t *list);

process_t *remove_foot(list_t *list);

int process_in_list(list_t *list, process_t *process);

void print_process(process_t *process);

void print_list(list_t *list);