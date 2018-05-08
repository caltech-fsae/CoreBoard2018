#ifndef MACHINE_H_
#define MACHINE_H_

#include <stdlib.h>

typedef int StateId;

typedef int EventId;

typedef struct state
{
	StateId id_;
	void(*state_loop_)(void);
} State;

typedef struct transition
{
	void(*function_)(void);
	StateId destination_state_;
} Transition;

typedef struct {
	int initialized_;
	StateId current_state_;
	StateId previous_state_;
	int num_states_;
	int num_events_;
	State *states_;
	StateId state_index_;
	EventId event_index_;
	Transition *table_;
} StateMachine;

void MakeStateMachine(StateMachine *machine, int num_states, int num_events);
StateId MakeState(StateMachine *machine, void(*state_loop)(void));
EventId MakeEvent(StateMachine *machine);
void AddEvent(StateMachine *machine, StateId origin_state, EventId event, StateId destination_state, void(*function)(void));
void InitStateMachine(StateMachine *machine, StateId start_state);
void RunEvent(StateMachine *machine, EventId event);
void RunState(StateMachine *machine);

#endif /* MACHINE_H_ */
