/* ----==== AIRCRAFT.CPP ====---- */

#include <windows.h>
#include <math.h>
#include <gl\gl.h>
#include "aircraft.h"
#include "main.h"

/*-----------------
---- VARIABLES ----
-----------------*/

extern unsigned int			Font1, Font2;
extern bool					ButtonDown[256];

extern Sim_Timer_Type		Timer;
extern Sim_Options_Type		Options;
extern Environment_Type		Environment;
extern Aircraft_Type		Aircraft;

/*-----------------
---- FUNCTIONS ----
-----------------*/

////////// class Aircraft_Type //////////

double Aircraft_Type::GetCl(void)
{
	double Cl = 0;

	switch (Flaps) {
		case 0:
			if (AOA <= 8.2f) {
				Cl = 0.18936f;
				Stalled = false;
			} else {
				Cl = -0.085106383f * (AOA - 8.2f) + 0.8872323406f;
				Stalled = true;
			}
			break;

		case 1:
			if (AOA <= 13.0f) {
				Cl = 0.15936f;
				Stalled = false;
			} else {
				Cl = -0.085106383f * (AOA - 13.0f) + 1.265742979f;
				Stalled = true;
			}
			break;

		case 5:
			if (AOA <= 13.0f) {
				Cl = 0.25936f;
				Stalled = false;
			} else {
				Cl = -0.085106383f * (AOA - 13.0f) + 1.365742979f;
				Stalled = true;
			}
			break;

		case 15:
			if (AOA <= 11.9f) {
				Cl = 0.47936f;
				Stalled = false;
			} else {
				Cl = -0.085106383f * (AOA - 11.9f) + 1.492125958f;
				Stalled = true;
			}
			break;

		case 20:
			if (AOA <= 11.6f) {
				Cl = 0.57936f;
				Stalled = false;
			} else {
				Cl = -0.085106383f * (AOA - 11.6f) + 1.566594043f;
				Stalled = true;
			}
			break;

		case 25:
			if (AOA <= 11.8f) {
				Cl = 0.61936f;
				Stalled = false;
			} else {
				Cl = -0.085106383f * (AOA - 11.8f) + 1.623615319f;
				Stalled = true;
			}
			break;

		case 30:
			if (AOA <= 9.8f) {
				Cl = 0.83936f;
				Stalled = false;
			} else {
				Cl = -0.085106383f * (AOA - 9.8f) + 1.673402553f;
				Stalled = true;
			}
			break;
	}
	
	if (!Stalled) Cl += 0.085106383f * AOA;
	
	return Cl;
}

double Aircraft_Type::GetCd(const double _Cl)
{
	double Cd = 0;
	double UseCl = _Cl;
	double a, b, c, d;
	a = b = c = d = 0;

	switch (Flaps) {
		case 0:
			a = 2.878692905f;
			b = -4.546656239f;
			c = 2.812087819f;
			d = -0.3518764486f;										// If stalled Cd will continue upward while
			if (Stalled) UseCl = (0.085106383f * AOA) + 0.18936f;	// Cl goes down, so Cl must be recalculated as
			break;													// continuing upward after stall here

		case 1:
			a = 0.1410590129f;
			b = 0.3621406002f;
			c = -0.0436267954f;
			d = 0.2200744058f;
			if (Stalled) UseCl = (0.085106383f * AOA) + 0.15936f;
			break;

		case 5:
			a = -0.1211193593f;
			b = 0.9892916978f;
			c = -0.5344483447f;
			d = 0.3682719406f;
			if (Stalled) UseCl = (0.085106383f * AOA) + 0.25936f;
			break;

		case 15:
			a = 0.2168053818f;
			b = -0.0118699978f;
			c = 0.298011024f;
			d = 0.2170358612f;
			if (Stalled) UseCl = (0.085106383f * AOA) + 0.47936f;
			break;

		case 20:
			a = 0.2368685919f;
			b = -0.1475008157f;
			c = 0.4988533831f;
			d = 0.1723374433f;
			if (Stalled) UseCl = (0.085106383f * AOA) + 0.57936f;
			break;

		case 25:
			a = 0.1111512362f;
			b = 0.3392769624f;
			c = -0.1792366348f;
			d = 0.8362220146f;
			if (Stalled) UseCl = (0.085106383f * AOA) + 0.61936f;
			break;

		case 30:
			a = 0.3779309184f;
			b = -0.7726574735f;
			c = 1.190838667f;
			d = 0.5464491137f;
			if (Stalled) UseCl = (0.085106383f * AOA) + 0.83936f;
			break;
	}

	Cd = (a * UseCl * UseCl * UseCl) + (b * UseCl * UseCl) + (c * UseCl) + d;
	if (Flaps == 25 || Flaps == 30) Cd -= 0.1f;			// The drag polar curves for flaps 25 and 30 were with gear down
	if (Gear) Cd += 0.1f;								// while the curves for other flap positions were with gear up

	return Cd;
}

inline double Aircraft_Type::GetThrust(const double _Sigma, const double _PressRatio,const double _Mach1)
{
	double GrossThrust = (63500.0f * NumEngines) * (0.235f * Mach * Mach * Mach + 0.607f * Mach * Mach + 1);// * _PressRatio;

	return GrossThrust * pow(N1 / 100.0f, 5.0f) * pow(_Sigma, 0.8f);

//	double Mach1 = _Mach1 * 1.687777777f;	// Speed of sound in ft/s

//	double GrossThrust = (63500.0f * NumEngines) * pow(N1 / 100.0f, 5.0f) * (0.235f * Mach * Mach * Mach + 0.607f * Mach * Mach + 1) * _PressRatio;

//	double RamDrag = (0.6f * Mach * Mach * Mach + 1 * Mach) * Mach1 * (PI*3*3) * 34.0f * _PressRatio / 32.2f;

//	return GrossThrust - RamDrag;
}

void Aircraft_Type::Update(void)
{
	///// Get timing multipliers

	double TimeFixSec = Timer.TimeFix * Options.SimRate;
	double TimeFixHr  = Timer.TimeFix * Options.SimRate / 3600.0f;

	///// Get Environment Variables

	if (DAlt < 37000) {
		OAT = 15.0f - (STD_DELTA_TEMP * (DAlt * 0.001f));	// Get the outside air temperature at current density altitude
	} else { 
		OAT = -56.5f;										// Temp is constant -56.5 in stratosphere
	}

	double TempRatio = (273.0f + OAT) / STD_TEMP_K;			// Get the temperature ratio

	if (DAlt > 37000) {
		OAP = 473.1f * exp(1.73f - (0.000048 * DAlt));		// Get the standard pressure in the lower stratosphere
	} else {
		OAP = STD_PRESS_PSF * pow(TempRatio, 5.256);		// Get the standard pressure in the troposphere
	}

	double PressRatio = OAP / STD_PRESS_PSF;				// Get the pressure ratio

	double Sigma = PressRatio / TempRatio;					// Get density ratio (sigma)

	double Mach1_KTAS = 661.74f * sqrt(TempRatio);			// Get speed of sound in Knots TAS

	///// Resolve Forces

	double CosVal = cos(Pitch * DEGTORAD);
	double SinVal = sin(Pitch * DEGTORAD);
	double CosVal2 = cos(FPA * DEGTORAD);
	double SinVal2 = sin(FPA * DEGTORAD);

	double qS = Sigma * TAS * TAS / 295.0f * 3129.0f;		// Wing Area = 3129 sq.ft.
	
	double Cl = GetCl();
	double Lift = Cl * qS;									// Lift = Cl * q * S
	if (Lift < 0) Lift = 0; // TEMP
	double LiftX = Lift * -SinVal2;
	double LiftY = Lift * CosVal2;
///// TEMP
	glColor3f(0,0.9f,0);
	PositionText(10,580);
	Print(Font1,"LIFT:  %0.1f",Lift);
/////

	double Cd0 = 0.1f + (Gear == true ? 0.15f : 0);
	double Cd = Cd0 + (Cl * Cl) / (PI * 9.3f * 0.85f);//GetCd(Cl);
	double Drag = Cd * qS;									// Drag = Cd * q * S
	if (Drag < 0) Drag = 0; // TEMP
	double DragX = Drag * -CosVal2;
	double DragY = Drag * -SinVal2;
///// TEMP
	double OldDrag = GetCd(Cl) * qS;
	PositionText(10,560);
	Print(Font1,"DRAG:  %0.1f",Drag);
	
	PositionText(10,440);
	Print(Font1,"OLDDRAG:  %0.1f", OldDrag);
/////

	double Thrust = GetThrust(Sigma,PressRatio,Mach1_KTAS);
	double ThrustX = Thrust * CosVal;
	double ThrustY = Thrust * SinVal;
///// TEMP
	PositionText(10,540);
	Print(Font1,"THRUST:  %0.1f",Thrust);
/////

	double ForcesX = ThrustX + LiftX + DragX;
	double ForcesY = ThrustY + LiftY + DragY - Weight;
	if (AAlt <= 0) {
		AAlt = 0;
		if (ForcesY < 0) ForcesY = 0;
	}
///// TEMP
	PositionText(10,520);
	Print(Font1,"FORCES-X:  %0.1f       FORCES-Y:  %0.1f", ForcesX, ForcesY);
/////

	double inv_mass = 1.0f / (Weight / 32.2f);				// Get (1 / mass) of the aircraft
	double ax = ForcesX * inv_mass;							// Find acceleration along the x axis (horizontal)
	double ay = ForcesY * inv_mass;							// Find acceleration along the y axis (vertical)

	///// Find Velocity Vector

	Vx += ax * TimeFixSec;									// Update velocity vector in fps
	Vy += ay * TimeFixSec;

///// TEMP
	PositionText(10,500);
	Print(Font1,"VX:  %0.1f       VY:  %0.1f", Vx, Vy);
/////

	TAS = sqrt((Vx * Vx) + (Vy * Vy)) * 0.5924950625f;		// TAS is the magnitude of the velocity vector in kts
	TAS -= Environment.Wind_Component;						// 3600 / 6076 = 0.5924950625
	if (TAS < 0) TAS = 0;

	///// Get Altitudes

	if (AAlt == 0) {										// If we are on or hit the ground
		if (Vy < 0) Vy = 0;
		if (Pitch < 0) Pitch = 0;
		if (EAS < 100 && Pitch > 0) Pitch = 0;
		if (Bank != 0) Bank = 0;
		IAlt = AAlt + Environment.Ground_Level_Alt;
	}

	VS = Vy * 60;											// Get vertical speed in fpm

	IAlt += Vy * TimeFixSec;								// Altitude

	AAlt = IAlt - Environment.Ground_Level_Alt;				// Height above ground

	if (Environment.DAlt_Const) {
		DAlt = Environment.DAlt_Init_Set;					// If constant density altitude
	} else {
		DAlt = IAlt + Environment.DAlt_Init_Set;			// If variable density altitude
	}

	///// Find Flight Path Angle and AOA

	if (Vx == 0) Vx = 0.000001f;
	FPA = atan(Vy / Vx) * RADTODEG;							// Find the flight path angle
	
/*	double AOI = 0;											// Get the angle of incidence
	switch (Flaps) {
		case 1:
			AOI = 0.1f;
			break;
		case 5:
			AOI = 0.5f;
			break;
		case 15:
			AOI = 1.5f;
			break;
		case 20:
			AOI = 2.0f;
			break;
		case 25:
			AOI = 2.5f;
			break;
		case 30:
			AOI = 3.0f;
			break;
	}*/
	
	AOA = Pitch - FPA;// + AOI;								// Find the AOA

	///// Get Speeds

	EAS = TAS * sqrt(Sigma);								// Get EAS from TAS
	if (EAS < 0) EAS = 0;
	Mach = TAS / Mach1_KTAS;								// Get Mach number
	
	GS = (Vx * 0.5924950625f);								// Get ground speed from the velocity along x axis and wind
	if (GS < 0) GS = 0;

	///// Get Distance Travelled
	
	Dist += GS * TimeFixHr;									// Update the distance travelled
///// TEMP
	PositionText(10,460);
	Print(Font1,"DIST:  %0.3f NM", Dist);
/////

}

void Aircraft_Type::Input(void)
{
	// Pitch
	if (KeyDown(VK_DOWN)) {
		if (!ButtonDown[VK_DOWN]) {
			ButtonDown[VK_DOWN] = true;

			Pitch += 1.0f;
			if (Pitch > 90) Pitch = 90;
		}
	} else ButtonDown[VK_DOWN] = false;

	if (KeyDown(VK_UP)) {
		if (!ButtonDown[VK_UP]) {
			ButtonDown[VK_UP] = true;

			Pitch -= 1.0f;
			if (Pitch < -90) Pitch = -90;
		}
	} else ButtonDown[VK_UP] = false;

	// Throttle
	if (KeyDown(VK_RIGHT)) {
		if (!ButtonDown[VK_RIGHT]) {
			ButtonDown[VK_RIGHT] = true;

			N1 += 3.0f;
			if (N1 > 100) N1 = 100;
		}
	} else ButtonDown[VK_RIGHT] = false;

	if (KeyDown(VK_LEFT)) {
		if (!ButtonDown[VK_LEFT]) {
			ButtonDown[VK_LEFT] = true;

			N1 -= 3.0f;
			if (N1 < 45) N1 = 45;
		}
	} else ButtonDown[VK_LEFT] = false;

	// Flaps
	if (KeyDown(VkKeyScan('['))) {
		if (!ButtonDown[VkKeyScan('[')]) {
			ButtonDown[VkKeyScan('[')] = true;

			switch (Flaps) {
				case 0:
					Flaps = 0;
					break;
				case 1:
					Flaps = 0;
					break;
				case 5:
					Flaps = 1;
					break;
				case 15:
					Flaps = 5;
					break;
				case 20:
					Flaps = 15;
					break;
				case 25:
					Flaps = 20;
					break;
				case 30:
					Flaps = 25;
					break;
			}
		}
	} else ButtonDown[VkKeyScan('[')] = false;

	if (KeyDown(VkKeyScan(']'))) {
		if (!ButtonDown[VkKeyScan(']')]) {
			ButtonDown[VkKeyScan(']')] = true;

			switch (Flaps) {
				case 0:
					Flaps = 1;
					break;
				case 1:
					Flaps = 5;
					break;
				case 5:
					Flaps = 15;
					break;
				case 15:
					Flaps = 20;
					break;
				case 20:
					Flaps = 25;
					break;
				case 25:
					Flaps = 30;
					break;
				case 30:
					Flaps = 30;
					break;
			}
		}
	} else ButtonDown[VkKeyScan(']')] = false;

	// Gear
	if (KeyDown('G')) {
		if (!ButtonDown['G']) {
			ButtonDown['G'] = true;

			Gear = !Gear;
		}
	} else ButtonDown['G'] = false;

}

Aircraft_Type::Aircraft_Type()
{
	Weight = 227300; //450000
	Flaps = 5;
	Gear = true;
	Brakes = false;
	Spoilers = false;
	Stalled = false;

	Vx = Vy = 0;
	CAS = EAS = TAS = GS = Mach = 0;
	Dist = 0;
	VS = 0;
	IAlt = 0;
	DAlt = 0;
	AAlt = 0;

	Pitch = Bank = AOA = FPA = 0;
	PitchRate = 2.0f;

	NumEngines = 2;
	N1 = 45.0;

	Autothrottle = false;
	Autothrottle_Set_IAS = 0;
	Autothrottle_Set_Mach = 0;

	Autopilot_Alt_Hold = false;
	Autopilot_Alt_Set = 0;
	Autopilot_VS_Hold = false;
	Autopilot_VS_Set = 0;
	Autopilot_IAS_Hold = false;
	Autopilot_IAS_Hold_Set = 0;
	Autopilot_Mach_Hold = false;
	Autopilot_Mach_Hold_Set = 0;

	OAT = OAP = 0;
}

