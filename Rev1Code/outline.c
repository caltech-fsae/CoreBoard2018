typedef enum
{
	WAIT         // waiting for driver start (brake + start button)
    DRIVE        // normal driving state
	START_BRAKE  // waiting for driver start (start button)
	RST_FAULT    // entered resettable fault state
	NO_RST_FAULT // entered non-resettable fault state
}StateType;

typedef struct
{
	StateType State;
	void (*func) (void);
}StateMachineType;

StateMachineType StateMachine[] =
{
	{ WAIT, wait_loop },
	{ DRIVE, drive_loop },
	{ START_BRAKE, start_brake_loop },
	{ RST_FAULT, rst_fault_loop },
	{ NO_RST_FAULT, no_rst_fault_loop }
};

StateType SmState = WAIT;

void wait_loop(void);
void drive_loop(void);
void start_brake_loop(void);
void rst_fault_loop(void);
void no_rst_fault_loop(void);


// need to store BRAKE_VAL, ACEL_VAL to check for changes

void wait_loop(void)
{
	while(no_transition_req) // after setting to 0, update SmState and break
	{
	    // check for CAN
	    // IGNORE: PEDAL_ACEL
	    //         PWR_DRAW if < 80 kW (how will this data be packaged, like what units - ask MC team)

	    // ACCEPT: PEDAL_BRAKE if (value changes, or above threshold - ask IO team) -> START_BRAKE, call brake_light_on(), send MC brake value, send CAN CAR_STATE update
	    //         PWR_DRAW if > 80 kW -> do not change state, send CAN power limit request (ask MC team what they want)
	    //         BPPC -> RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BPPC fault? - change constants)
	    //         BMS_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BMS fault?)
	    //         IMD_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an IMD fault?)
	    //         BSPD_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BSPD fault?)
	    //         APPS_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an APPS fault?)
	    //         BSE_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an BSE fault?)

	    // do not check GPIO start button, not relevant in WAIT      
	}
}

void drive_loop(void)
{
	while(no_transition_req) // after setting to 0, update SmState and break
	{
	    // check for CAN        

	    // ACCEPT: PEDAL_BRAKE if (value changes, or above threshold - ask IO team) -> call brake_light_on(), send CAN message to MC with new value
	    //         PEDAL_ACEL if (value changes, or above threshold - ask IO team) -> do not change state, send CAN message to MC with new value 
	    //         PWR_DRAW if > 80 kW -> do not change state, send CAN power limit request (ask MC team what they want)
	    //         BPPC -> RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BPPC fault? - change constants)
	    //         BMS_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BMS fault?)
	    //         IMD_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an IMD fault?)
	    //         BSPD_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BSPD fault?)
	    //         APPS_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an APPS fault?)
	    //         BSE_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an BSE fault?)

	    // do not check GPIO start button, not relevant in DRIVE      
	}
}

void start_brake_loop(void)
{
	while(no_transition_req) // after setting to 0, update SmState and break
	{
	    // check for CAN      

	    // ACCEPT: PEDAL_BRAKE if (value changes, or above threshold - ask IO team) -> do not change state, send CAN message to MC with new value
	    //         PEDAL_ACEL if (value changes, or above threshold - ask IO team) -> WAIT, send CAN message to MC with new value 
	    //         PWR_DRAW if > 80 kW -> do not change state, send CAN power limit request (ask MC team what they want)
	    //         BPPC -> RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BPPC fault? - change constants)
	    //         BMS_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BMS fault?)
	    //         IMD_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an IMD fault?)
	    //         BSPD_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BSPD fault?)
	    //         APPS_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an APPS fault?)
	    //         BSE_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an BSE fault?)

	    // check GPIO for start button pressed
	    //         if value read from pin = active (aka start button is pressed) -> DRIVE, call RTDS_on() send CAN CAR_STATE update
	    //         else ignore  
	}
}

void rst_fault_loop(void)
{
	while(no_transition_req) // after setting to 0, update SmState and break
	{
	    // check for CAN   
	    // IGNORE: PEDAL_BRAKE
	    //         PEDAL_ACEL
	    //         PWR_DRAW
	    //         BPPC   

	    // ACCEPT: BMS_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BMS fault?)
	    //         IMD_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an IMD fault?)
	    //         BSPD_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is a BSPD fault?)
	    //         APPS_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an APPS fault?)
	    //         BSE_NO_RST -> NO_RST_FAULT, send CAN CAR_STATE update (w/ indication that this is an BSE fault?)

	    // do not check GPIO start button, not relevant in RST_FAULT    
	}
}

void no_rst_fault_loop(void)
{
	while(no_transition_req) // after setting to 0, update SmState and break
	{ 
	    // IGNORE: literally everything, can't escape this state, it's non-resettable!
	}
}