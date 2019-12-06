/* ----==== AIRCRAFT.H ====---- */

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

/*------------------
---- STRUCTURES ----
------------------*/

class Aircraft_Panel_Type;

class Aircraft_Type {
	friend Aircraft_Panel_Type;

	private:
		int		Weight;		// Weight in pounds
		int		Flaps;		// Flap angle set in increments of 0, 1, 5, 15, 20, and 30
		bool	Gear;		// Gear down (1)
		bool	Brakes;		// Wheel brakes on (1)
		bool	Spoilers;	// Spoilers on (1)
		bool	Stalled;	// Stalled (1)

		double	Vx, Vy;		// Velocity vector in fps
		double	CAS;		// Calibrated airspeed
		double	EAS;		// Equivalent airspeed
		double	TAS;		// True airspeed
		double	GS;			// Ground speed
		double	Mach;		// Mach number
		double	Dist;		// Distance travelled in nm
		double	VS;			// Vertical speed in fpm
		double	IAlt;		// Indicated altitude
		double	DAlt;		// Density altitude
		double	AAlt;		// Absolute altitude (height above ground)

		double	Pitch;		// Pitch angle
		double	Bank;		// Bank angle
		double	AOA;		// Angle of attack
		double	FPA;		// Flight path angle
		double	PitchRate;	// Pitch rate in degrees/second

		unsigned char	NumEngines;			// Number of engines operating (0 - 2)
		double			N1;					// Throttle setting %N1 (45 - 100)

		bool	Autothrottle;				// Autothrottle On/Off
		int		Autothrottle_Set_IAS;		// Speed setting
		double	Autothrottle_Set_Mach;		// Mach setting

		bool	Autopilot_Alt_Hold;			// Altitude hold mode on
		int		Autopilot_Alt_Set;			// Altitude setting
		bool	Autopilot_VS_Hold;			// Vertical speed hold
		int		Autopilot_VS_Set;			// Vertical speed setting
		bool	Autopilot_IAS_Hold;			// Hold airpseed by varying pitch
		int		Autopilot_IAS_Hold_Set;		// Airspeed setting
		bool	Autopilot_Mach_Hold;		// Hold mach by varying pitch
		int		Autopilot_Mach_Hold_Set;	// Mach setting

		double	OAT;						// Outside air temperature (degrees Celcius)
		double	OAP;						// Outside air pressure (inches Hg)

	public:
		double			GetCl(void);				// Find the coefficient of lift
		double			GetCd(const double);		// Find the coefficient of drag
		inline double	GetThrust(const double, const double, const double);	// Find the thrust

		void			Update(void);		// Updates variables
		void			Input(void);		// Handles input

		Aircraft_Type();					// Constructor
		~Aircraft_Type() {}					// Destructor
};

#endif