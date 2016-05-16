/* Linked list of processes images in memory */

typedef struct m_process m_process_t;

struct m_process {
    /* Process data */
    int time_allocated;
    int process_id;
    int memsize;
    int relative_pos; /* Relative number of address after the top address*/ 
    int relative_end; /* Relative end address*/
   
    m_process_t *previous;
    m_process_t *next;

};

typedef struct {
    int size;
    int num_process;
    int used_memory;
    m_process_t *head;
    m_process_t *foot;  
} memory_t;
  


memory_t *make_empty_memory(int size);

int is_empty_memory(memory_t *list);

void free_memory(memory_t *list);

memory_t *insert_process(memory_t *memory, int time_allocated, int process_id, int memsize);

memory_t *insert_before(memory_t *memory, m_process_t *process, int time_allocated, int process_id, int memsize);

memory_t *insert_at_head_m(memory_t *list, int time_allocated, int process_id, int memsize);

memory_t *insert_at_foot_m(memory_t *list, int time_allocated, int process_id, int memsize);

memory_t *remove_process(memory_t *memory, m_process_t *process);

memory_t *remove_process_id(memory_t *memory, int process_id);

int check_process_id(memory_t *memory, int process_id);

void compare_size(m_process_t *curr, m_process_t *largest);

m_process_t *find_process(memory_t *memory, int process_id);

int num_holes(memory_t *memory);

void print_m_process(m_process_t *process);

void print_memory(memory_t *memory);