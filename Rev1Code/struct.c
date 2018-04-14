#include <stdlib.h>

typedef enum
{
    WAIT           ,// waiting for driver start (brake + start button)
    DRIVE          ,// normal driving state
    START_BRAKE    ,// waiting for driver start (start button)
    RST_FAULT      ,// entered resettable fault state
    NO_RST_FAULT   // entered non-resettable fault state
}StateType;

typedef enum
{
    START,
    PEDAL_ACEL         ,
    PEDAL_BRAKE_PUSHED  ,
    PEDAL_BRAKE_RELEASED ,
    PWR_80  ,
    BPPC_RST,
    IMD_NO_RST,
    BSPD_NO_RST,
    APPS_NO_RST,
    BSE_NO_RST,
    BMS_NO_RST 
}EventType;


typedef struct transition
{
    void (*function)(void);
    StateType next_state;
} Transition;

typedef struct  {
  StateType current_state;
  int num_states;
  int num_events;
  Transition *table;
} statemachine;

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
