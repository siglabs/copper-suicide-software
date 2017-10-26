/*
 * Any event methods/function addition will start with 'e_'
 * 
 */
#ifndef __LIB_EVENT__
#define __LIB_EVENT__


#include <stdlib.h>

#define AUTO_ASSIGN_EID -99


namespace sig_events{

// TODO(shashank/sai): Add description and usage schemes.
enum e_priority_t{
    TIME_CRITICAL = 0,
    SYS_ERROR = 1,
    NORMAL_LONG = 2,
    NORMAL_QUICK =3,
    HIGH_QUICK = 4, 
    HIGH_LONG = 5
};

// Definition of our event.
enum e_status_t{
    EXECUTING = 0,
    BLOCK_ISR = 1,
    BLOCK_SCHEDULER = 2,
    IN_QUEUE = 3,
	FINISHED = 4
};

// This struct is a demonstration of what a typical argument should look like.
struct sample_arg{
	int event_id;
	char *even_desc;
};

// function for callback
typedef void(*event_cb_t)(int event, void *userdata);


// Declaration of Event structure
struct event_b{
	int event_id;
    e_priority_t priority;
    e_status_t status;

    event_cb_t event_handler;

    void *arg_data;

    struct event_b *next, *prev;

};

class EventManager {

	static int num_event;
	static int num_serviced;
	static int num_critical_queue;
	static int num_critical_serviced;
	public:
		struct event_b *event_list;
		struct event_b *buffer;

		int head,tail;
		int buffer_size;

		// constructor
		EventManager();
		EventManager(int buffer_size);
		~EventManager();
		// Getting Events Methods.
		struct event_b e_pop();
		struct event_b e_pop(int id);

		//Circular Buffer Events, ideally follows same input/output as lists
		struct event_b* e_dequeue_circBuf();
		int e_push_circBuf(struct event_b new_event);
		int is_Bufempty();


        // create event
        // TODO: ensure it sets event_id.
		struct event_b e_create(e_priority_t priority=NORMAL_LONG,
								e_status_t status=IN_QUEUE, event_cb_t func_to_exec={},
								void *arg_data={});
		// returns event id on SUCCESS else -1.
		int e_push(struct event_b new_event);
        
		// ideally the argument should be this->e_pop()
		int e_exec(struct event_b *runnable);

};

}// namespace sig_events
#endif __LIB_EVENT__

