/* ----==== MAIN.H ====---- */

#ifndef MAIN_H
#define MAIN_H

/*---------------
---- DEFINES ----
---------------*/

#define PI					3.1415926535897932384626433f
#define DEGTORAD			0.01745329252f
#define RADTODEG			57.295779513f

#define STD_TEMP_K			288.0f	// Standard Temp Kelvin
#define STD_DELTA_TEMP		1.9324f	// Standard Temp change / 1000 ft up to 37,000 ft
#define STD_PRESS_PSF		2116.2f	// Standard Pressure lbs./sq.ft.

#define KeyDown(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/*-----------------
---- FUNCTIONS ----
-----------------*/

void PositionText(int x, int y);
void Print(unsigned int Font, const char *Text, ...);

/*------------------
---- STRUCTURES ----
------------------*/

struct Sim_Timer_Type {
	_int64	TimerFrequency, CurrentTime, LastTime;
	double	TimeFix, TimerFrequencyFix;

	void GetTimeFix(void);
	Sim_Timer_Type();
	~Sim_Timer_Type() {}
};

struct Sim_Options_Type {
	int		Scale_Start_Dist;	// Starting distance of the display
	int		Scale_Start_Alt;	// Starting altitude of the display
	int		Scale_Full_Dist;	// Full scale distance of the display
	int		Scale_Full_Alt;		// Full scale altitude of the display
	bool	Ground_Effect;		// Ground effect on (1)
	bool	Pause_On_Input;		// Pause the sim when inputting a value (1) or not (0)
	bool	Paused;				// Paused
	bool	Active;				// Window active
	double	SimRate;			// Simulation rate multiplier, 1 for real time
};

struct Environment_Type {
	int		Ground_Level_Alt;	// Ground level altitude MSL
	bool	DAlt_Const;			// Constant (1) or variable (0) density altitude
	int		DAlt_Init_Set;		// Constant density altitude setting or sea level density altitude for variable
	int		Wind_Component;		// Positive means tailwind, negative means headwind
};

#endif