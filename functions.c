#include "functions.h"

// Functions for state transitions. Takes in CAN message and current state. Function names in form of SmState_Event.
void PEDAL_ACEL()
{
    // occurs when acel value changed enough to care
}
void PEDAL_BRAKE_PUSHED()
{
    // occurs when brake pressed enough to turn on brake light
}
void PEDAL_BRAKE_RELEASED()
{
    // occurs when brake released, light should be turned off
}
void PWR_80()
{
    // occurs when power over 80 kW
}
void BPPC_RST()
{
    // 
}
void IMD_NO_RST()
{
    //
}
void BSPD_NO_RST()
{
    //
}
void APPS_NO_RST()
{
    //
}
void BSE_NO_RST()
{
    //
}
void BMS_NO_RST()
{
    //
}

void do_nothing()
{
    // return
}

