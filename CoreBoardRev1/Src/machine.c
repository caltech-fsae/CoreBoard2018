/*
 * machine.c
 *
 *  Created on: Mar 6, 2018
 *      Author: shwet
 */
#include "machine.h"

void make_state_machine(statemachine * machine, int num_states, int num_events, StateType current_state)
{
    machine->current_state = current_state;
    machine->num_states = num_states;
    machine->num_events = num_events;
    machine->table = malloc(num_states * num_events * sizeof(Transition));
}

void add_tuple(statemachine * machine, StateType state, EventType event, StateType next_state, void (*function)(void))
{
    Transition t;
    t.next_state = next_state;
    t.function = function;
    machine->table[state * machine->num_states + event] = t;
}

void run_event(statemachine * machine, EventType event)
{
    machine->table[(machine->current_state) * machine->num_states + event].function();
    machine->current_state = machine->table[(machine->current_state) * machine->num_states + event].next_state;
}

