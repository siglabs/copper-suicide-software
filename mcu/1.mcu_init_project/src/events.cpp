#include "events.h"


namespace sig_events {

int EventManager::num_event = 0;
int EventManager::num_serviced = 0;
int EventManager::num_critical_queue = 0;
int EventManager::num_critical_serviced = 0;

// TODO(shashank): Restructure in the order of severity.
EventManager::EventManager(){
	this->event_list = new event_b;
	this->num_event = 0;
	this->num_serviced = 0;
	this->num_critical_queue = 0;
	this->num_critical_serviced = 0;
//	this->head = 0;
//	this->tail = 0;
}

EventManager::EventManager(int buffer_size){
	//this->event_list = new event_b;
	this->buffer_size = buffer_size;
	this->buffer = new event_b[buffer_size];
	this->num_event = 0;
	this->num_serviced = 0;
	this->num_critical_queue = 0;
	this->num_critical_serviced = 0;
	this->head = 0;
	this->tail = 0;
}

struct event_b EventManager::e_pop(){
	struct event_b *return_val = this->event_list;
	delete(this->event_list);
	this->event_list = this->event_list->next;

	return *return_val;
}

struct event_b EventManager::e_pop(int id){

	struct event_b *temp = this->event_list;
	while(temp->event_id != NULL){
		if(temp->event_id == id)
			return *temp;
		else
			temp = temp->next;
	}
	return *temp;
}

// TODO(shashank): complete this method.
struct event_b e_create(e_priority_t priority=NORMAL_LONG,
						e_status_t status=IN_QUEUE, event_cb_t func_to_exec={},
						void *arg_data={}){
	struct event_b j;
	return j;
}

// TODO(shashank/sai): change the implementation to Circular buffer.
int EventManager::e_push(struct event_b new_event){

	struct event_b *eve = new event_b;

	eve->arg_data = new_event.arg_data;
	eve->event_handler = new_event.event_handler;
	eve->priority = new_event.priority;
	eve->status = new_event.status;

	if(new_event.event_id == AUTO_ASSIGN_EID)
		eve->event_id = this->num_event++;
	else
		eve->event_id = new_event.event_id;

	eve->next = this->event_list;

	this->event_list = eve;
	//this->num_event++;
	if (this->event_list->priority <= SYS_ERROR)
		(this->num_critical_queue++);

	return this->num_event;
}

// TODO(sai): change the implementation to Circular buffer.
int EventManager::e_push_circBuf(struct event_b new_event){

	// TODO(shashank): verify if this is what you intend to do :)
	new_event.status = IN_QUEUE;
	// next is where head will point to after this write.
	int next = this->head +1;

	if (next >= this->buffer_size)
	        next = 0;
	if (next == this->tail) // check if circular buffer is full
	        return -1;      //TODO Sync with errors in e_dequeue_circBuf

	this->buffer[this->head] = new_event;

	if(new_event.event_id == AUTO_ASSIGN_EID)
		this->buffer[this->head].event_id = this->num_event++;
	else
		this->buffer[this->head].event_id = new_event.event_id;

	this->head = next;
	return this->num_event;
}

struct event_b* EventManager::e_dequeue_circBuf(){

	struct event_b *x = NULL;
	if(this->head == this->tail)
		return x;              //TODO This needs to throw an error showing buffer is empty

	int next = this->tail +1;
	if(next >= this->buffer_size)
		next =0;

	x = &this->buffer[this->tail];
	this->tail = next;

	return x;
}

int EventManager::is_Bufempty()
{
	if(this->head==this->tail)
		return 1;
	else return 0;
}

// TODO(shashank): This method needs to set event status.
int EventManager::e_exec(struct event_b *runnable){

	struct event_b *temp = this->event_list;
	// TODO(shashank-CRITICAL):refine this logic and ensure you free the memory.
	if(this->num_critical_queue > 0){
		while(temp->priority > SYS_ERROR){
			temp =  temp->next;
		}
		if(temp->status == IN_QUEUE && temp->event_handler != NULL){
			runnable->status = EXECUTING;

			temp->event_handler(temp->event_id, temp->arg_data);
			temp->status = FINISHED;
		}
	}
	if(runnable->status == IN_QUEUE && runnable->event_handler != NULL){
		runnable->status = EXECUTING;
		runnable->event_handler(runnable->event_id, runnable->arg_data);
		runnable->status = FINISHED;
	} else
		return -1;
	return 1;

}

EventManager::~EventManager(){
	delete(this);
}

} // namespace sig_events
