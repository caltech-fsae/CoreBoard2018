#include "sm2.h"
#include <assert.h>
// #define DEBUG // why is brendan like this
void MakeStateMachine(StateMachine *machine, int num_states, int num_events)
{
	machine->initialized_ = 0;
	machine->state_index_ = 0;
	machine->event_index_ = 0;
	machine->num_states_ = num_states;
	machine->num_events_ = num_events;
	machine->table_ = malloc(num_states * num_events * sizeof(Transition));
	machine->states_ = malloc(num_states * sizeof(State));
}

void InitStateMachine(StateMachine *machine, StateId start_state) {
#ifdef DEBUG
	assert(machine->initialized_ == 0);
#endif // DEBUG
	machine->initialized_ = 1;
	machine->current_state_ = start_state;
}

StateId MakeState(StateMachine *machine, void(*state_loop)(void)) {
#ifdef DEBUG
	assert(machine->state_index_ < machine->num_states_);
#endif // DEBUG
	State s;
	s.id_ = machine->state_index_;
	s.state_loop_ = state_loop;
	machine->states_[machine->state_index_] = s;
	machine->state_index_++;
	return s.id_;
}

EventId MakeEvent(StateMachine *machine) {
#ifdef DEBUG
	assert(machine->event_index_ < machine->num_events_);
#endif // DEBUG
	return machine->event_index_++;
}

void AddEvent(StateMachine *machine, StateId origin_state, EventId event, StateId destination_state, void(*function)(void))
{
#ifdef DEBUG
	assert(event < machine->event_index_);
	assert(origin_state < machine->state_index_);
	assert(destination_state < machine->state_index_);
#endif // DEBUG
	machine->table_[origin_state * machine->num_events_ + event].destination_state_ = destination_state;
	machine->table_[origin_state * machine->num_events_ + event].function_ = function;
}

void RunEvent(StateMachine *machine, EventId event)
{
#ifdef DEBUG
	assert(event < machine->event_index_);
#endif  // DEBUG
	machine->table_[(machine->current_state_) * machine->num_events_ + event].function_();
	machine->current_state_ = machine->table_[(machine->current_state_) * machine->num_events_ + event].destination_state_;
}


void RunState(StateMachine *machine) {
#ifdef DEBUG
	assert(machine->initialized_ == 1);
#endif  // DEBUG
	machine->states_[machine->current_state_].state_loop_();
}
