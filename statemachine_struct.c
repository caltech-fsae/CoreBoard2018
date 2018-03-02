typedef struct 
{
    void (*function)(void);
    StateType next_state;
} Transition;

struct statemachinetype {
	StateType current_state;
	int num_states;
	int num_events;
	void *Transition table;
}


void makestatemachine(statemachine * machine, num_states, num_events);

void add_tuple(statemachine * machine, state, event, function);

void run_event(statemachine * machine, event);

void makestatemachine(statemachine * machine, num_states, num_events, StateType current_state)
{
    machine->current_state = current_state;
    machine->num_states = num_states;
    machine->num_events = num_events;
    machine->table = malloc(num_states * num_events * sizeof(Transition));
}

void add_tuple(statemachine * machine, state, event, StateType next_state * function)
{
    Transition t;
    t.next_state = next_state;
    t.function = function;
    machine->table[(state * event)] = t;
}

void run_event(statemachine * machine, event)
{
    machine->table[(machine->current_state) * event].function();
    machine->current_state = machine->table[(machine->current_state) * event].next_state;
}