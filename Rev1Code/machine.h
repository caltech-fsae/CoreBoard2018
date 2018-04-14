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
    BPPC_FLT,
    IMD_FLT,
    BSPD_FLT,
    APPS_FLT,
    BSE_FLT,
    BMS_FLT 
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

void make_state_machine(statemachine * machine, int num_states, int num_events, StateType current_state);
void add_tuple(statemachine * machine, StateType state, EventType event, StateType next_state, void (*function)(void));
void run_event(statemachine * machine, EventType event);