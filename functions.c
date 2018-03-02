// Functions for state transitions. Takes in CAN message and current state. Function names in form of SmState_Event.
void WAIT_PEDAL_BRAKE();
void WAIT_PEDAL_BRAKE_RELEASED();
void WAIT_BPPC_RST();
void WAIT_IMD_NO_RST();
void WAIT_BSPD_NO_RST();
void WAIT_APPS_NO_RST();
void WAIT_BSE_NO_RST();
void WAIT_BMS_NO_RST();

void DRIVE_PEDAL_ACEL();
void DRIVE_PEDAL_BRAKE();
void DRIVE_PEDAL_BRAKE_RELEASED();
void DRIVE_PWR_80();
void DRIVE_BPPC_RST();
void DRIVE_IMD_NO_RST();
void DRIVE_BSPD_NO_RST();
void DRIVE_APPS_NO_RST();
void DRIVE_BSE_NO_RST();
void DRIVE_BMS_NO_RST();

void START_BRAKE_START();
void START_BRAKE_PEDAL_ACEL();
void START_BRAKE_PEDAL_BRAKE();
void START_BRAKE_PEDAL_BRAKE_RELEASED();
void START_BRAKE_PWR_80();
void START_BRAKE_BPPC_RST();
void START_BRAKE_IMD_NO_RST();
void START_BRAKE_BSPD_NO_RST();
void START_BRAKE_APPS_NO_RST();
void START_BRAKE_BSE_NO_RST();
void START_BRAKE_BMS_NO_RST();

void RST_FAULT_PEDAL_BRAKE();
void RST_FAULT_IMD_NO_RST();
void RST_FAULT_BSPD_NO_RST();
void RST_FAULT_APPS_NO_RST();
void RST_FAULT_BSE_NO_RST();
void RST_FAULT_BMS_NO_RST();

void do_nothing();

// events in WAIT state
void WAIT_PEDAL_BRAKE()
{
    // change GPIO pin for brake_light_on()
    // send START_BRAKE_CAN
    // send_new_brake_CAN() (altho prob not)
}
void WAIT_PEDAL_BRAKE_RELEASED()
{
    // change GPIO pin for brake_ligth_off()
}
void WAIT_BPPC_RST()
{
    // send RST_FAULT_CAN
}
void WAIT_IMD_NO_RST()
{
    // send IMD_NO_RST_CAN
}
void WAIT_BSPD_NO_RST()
{
    // send BSPD_NO_RST_CAN
}
void WAIT_APPS_NO_RST()
{
    // send APPS_NO_RST_CAN
}
void WAIT_BSE_NO_RST()
{
    // send BSE_NO_RST_CAN
}
void WAIT_BMS_NO_RST()
{
    // send BMS_NO_RST_CAN
}

// events in DRIVE state
void DRIVE_PEDAL_ACEL()
{
    // send_new_acel_CAN() (altho prob not)
}
void DRIVE_PEDAL_BRAKE()
{
    // change GPIO pin for brake_light_on()
    // send_new_brake_CAN() (altho prob not)
}
void DRIVE_PEDAL_BRAKE_RELEASED()
{
    // change GPIO pin for brake_light_off()
}
void DRIVE_PWR_80()
{
    // send_current_lim_CAN()
}
void DRIVE_BPPC_RST()
{
    // send RST_FAULT_CAN
}
void DRIVE_IMD_NO_RST()
{
    // send IMD_NO_RST_CAN
}
void DRIVE_BSPD_NO_RST()
{
    // send BSPD_NO_RST_CAN
}
void DRIVE_APPS_NO_RST()
{
    // send APPS_NO_RST_CAN
}
void DRIVE_BSE_NO_RST()
{   
    // send BSE_NO_RST_CAN
}
void DRIVE_BMS_NO_RST()
{
    // send BMS_NO_RST_CAN
}

// events in START state
void START_BRAKE_START()
{
    // send DRIVE_CAN
}
void START_BRAKE_PEDAL_ACEL()
{
    // send WAIT_CAN
}
void START_BRAKE_PEDAL_BRAKE()
{
    // change GPIO pin for brake_light_on()
}
void START_BRAKE_PEDAL_BRAKE_RELEASED()
{
    // change GPIO pin for brake_light_off()
    // send WAIT_CAN
}
void START_BRAKE_PWR_80()
{
    // send_current_lim_CAN() 
    // send WAIT_CAN
}
void START_BRAKE_BPPC_RST()
{
    // send RST_FAULT_CAN
}
void START_BRAKE_IMD_NO_RST()
{
    // send IMD_NO_RST_CAN
}
void START_BRAKE_BSPD_NO_RST()
{
    // send BSPD_NO_RST_CAN
}
void START_BRAKE_APPS_NO_RST()
{
    // send APPS_NO_RST_CAN
}
void START_BRAKE_BSE_NO_RST()
{
    // send BSE_NO_RST_CAN
}
void START_BRAKE_BMS_NO_RST()
{
    // send BMS_NO_RST_CAN
}

// events in RST_FAULT state
void RST_FAULT_PEDAL_BRAKE()
{
    // change GPIO pin for brake_light_on()
    // send START_BRAKE_CAN
    // send_new_brake_CAN() (altho prob not)
}
void RST_FAULT_IMD_NO_RST()
{
    // send IMD_NO_RST_CAN
}
void RST_FAULT_BSPD_NO_RST()
{
    // send BSPD_NO_RST_CAN
}
void RST_FAULT_APPS_NO_RST()
{
    // send APPS_NO_RST_CAN
}
void RST_FAULT_BSE_NO_RST()
{
    // send BSE_NO_RST_CAN
}
void RST_FAULT_BMS_NO_RST()
{
    // send BMS_NO_RST_CAN
}

void do_nothing()
{
    // do nothing
}