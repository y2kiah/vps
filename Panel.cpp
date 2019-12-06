/* ----==== PANEL.CPP ====---- */

#include <windows.h>
#include <gl\gl.h>
#include "panel.h"
#include "aircraft.h"
#include "main.h"

/*-----------------
---- VARIABLES ----
-----------------*/

extern unsigned int			Font1, Font2;

extern Aircraft_Type		Aircraft;

/*-----------------
---- FUNCTIONS ----
-----------------*/

////////// class Aircraft_Panel_Type //////////

void Aircraft_Panel_Type::DrawAttInd(void)
{
	int HorizonLevel = 200 - (int)(Aircraft.Pitch * 3.0f);
	int PitchStart = HorizonLevel;
	if (PitchStart > 260) PitchStart = 260;
	else if (PitchStart < 140) PitchStart = 140;

	glBegin(GL_QUADS);
		// Background
		glColor3f(0.3f,0.3f,0.3f);
		glVertex2i(320,280);
		glVertex2i(480,280);
		glVertex2i(480,120);
		glVertex2i(320,120);
		// Blue
		glColor3f(0.1f,0.2f,0.4f);
		glVertex2i(325,275);
		glVertex2i(475,275);
		glVertex2i(475,125);
		glVertex2i(325,125);
		// Upper
		if (PitchStart < 260) {
			glColor3f(0.3f,0.7f,0.9f);
			glVertex2i(340,260);
			glVertex2i(460,260);
			glVertex2i(460,PitchStart);
			glVertex2i(340,PitchStart);
		}
		// Lower
		if (PitchStart > 140) {
			glColor3f(0.5f,0.35f,0.15f);
			glVertex2i(340,PitchStart);
			glVertex2i(460,PitchStart);
			glVertex2i(460,140);
			glVertex2i(340,140);
		}
	glEnd();

	glColor3f(1,1,1);
	
	// Horizon Line
	if (PitchStart > 140 && PitchStart < 260) {
		glBegin(GL_LINES);
			glVertex2i(340,PitchStart);
			glVertex2i(460,PitchStart);
		glEnd();
	}

	// Pitch Lines
	int PitchBar = 0;
	glBegin(GL_LINES);
		for (float p = 2.5f; p < 40.0f; p += 5.0f) {

			PitchBar = (int)(HorizonLevel + (p * 3.0f));
			if (PitchBar < 260 && PitchBar > 140) {
				glVertex2i(390,PitchBar);
				glVertex2i(410,PitchBar);
			}

			PitchBar = (int)(HorizonLevel - (p * 3.0f));
			if (PitchBar < 260 && PitchBar > 140) {
				glVertex2i(390,PitchBar);
				glVertex2i(410,PitchBar);
			}
		}

		for (p = 5.0f; p < 40.0f; p += 10.0f) {

			PitchBar = (int)(HorizonLevel + (p * 3.0f));
			if (PitchBar < 260 && PitchBar > 140) {
				glVertex2i(380,PitchBar);
				glVertex2i(420,PitchBar);
			}

			PitchBar = (int)(HorizonLevel - (p * 3.0f));
			if (PitchBar < 260 && PitchBar > 140) {
				glVertex2i(380,PitchBar);
				glVertex2i(420,PitchBar);
			}
		}

		for (p = 10.0f; p <= 90.0f; p += 10.0f) {

			PitchBar = (int)(HorizonLevel + (p * 3.0f));
			if (PitchBar < 260 && PitchBar > 140) {
				glVertex2i(365,PitchBar);
				glVertex2i(435,PitchBar);
			}

			PitchBar = (int)(HorizonLevel - (p * 3.0f));
			if (PitchBar < 260 && PitchBar > 140) {
				glVertex2i(365,PitchBar);
				glVertex2i(435,PitchBar);
			}
		}
	glEnd();

	// Pitch Numbers

	glScissor(340,140,120,120);
	glEnable(GL_SCISSOR_TEST);
	
	for (int pn = 10; pn <= 90; pn += 10) {

		PitchBar = HorizonLevel + (pn * 3);
		if (PitchBar < 260 && PitchBar > 140) {
			PositionText(349,PitchBar - 4);
			Print(Font1,"%d",pn);
			PositionText(437,PitchBar - 4);
			Print(Font1,"%d",pn);
		}

		PitchBar = HorizonLevel - (pn * 3);
		if (PitchBar < 260 && PitchBar > 140) {
			PositionText(349,PitchBar - 4);
			Print(Font1,"%d",pn);
			PositionText(437,PitchBar - 4);
			Print(Font1,"%d",pn);
		}
	}

	glDisable(GL_SCISSOR_TEST);

	// Airplane Symbol
	glColor3f(0.8f,0.8f,0.8f);

	// Dot
	glPointSize(4);
	glBegin(GL_POINTS);
		glVertex2i(400,200);
	glEnd();
	
	glBegin(GL_QUADS);
		// Left Wing
		glVertex2i(380,202);
		glVertex2i(380,198);
		glVertex2i(355,198);
		glVertex2i(355,202);
		// Left Winglet
		glVertex2i(380,202);
		glVertex2i(384,202);
		glVertex2i(384,194);
		glVertex2i(380,194);
		// Right Wing
		glVertex2i(420,202);
		glVertex2i(420,198);
		glVertex2i(445,198);
		glVertex2i(445,202);
		// Right Winglet
		glVertex2i(420,202);
		glVertex2i(416,202);
		glVertex2i(416,194);
		glVertex2i(420,194);
	glEnd();

	glColor3f(0,0,0);
	
	// Dot
	glPointSize(2);
	glBegin(GL_POINTS);
		glVertex2i(400,200);
	glEnd();

	glBegin(GL_QUADS);
		// Left Wing
		glVertex2i(381,201);
		glVertex2i(381,199);
		glVertex2i(356,199);
		glVertex2i(356,201);
		// Left Winglet
		glVertex2i(381,201);
		glVertex2i(383,201);
		glVertex2i(383,195);
		glVertex2i(381,195);
		// Right Wing
		glVertex2i(419,201);
		glVertex2i(419,199);
		glVertex2i(444,199);
		glVertex2i(444,201);
		// Right Winglet
		glVertex2i(419,201);
		glVertex2i(417,201);
		glVertex2i(417,195);
		glVertex2i(419,195);
	glEnd();

	// Write Stuff TEMP TEMP TEMP
	glColor3f(0,0.9f,0);
	PositionText(200,60);
	Print(Font1,"FPA:   %3.2f",Aircraft.FPA);

	PositionText(200,40);
	Print(Font1,"AOA:   %3.2f",Aircraft.AOA);

	PositionText(550,100);
	Print(Font2,"%0.1f%% N1 ",Aircraft.N1);

	PositionText(550,50);
	Print(Font1,"FLAPS: %d",Aircraft.Flaps);

	PositionText(550,30);
	if (Aircraft.Gear) Print(Font1,"GEAR: DOWN");
	else Print(Font1,"GEAR: UP");

	PositionText(550,200);
	Print(Font2,"%0.0f FT MSL",Aircraft.IAlt);

	PositionText(550,160);
	Print(Font2,"%0.0f FPM",Aircraft.VS);

	if (Aircraft.Stalled) {
		PositionText(200,140);
		Print(Font1,"STALLED");
	}
}

void Aircraft_Panel_Type::DrawMFD1(void)
{
	glBegin(GL_QUADS);
		// Background
		glColor3f(0.3f,0.3f,0.3f);
		glVertex2i(320,110);
		glVertex2i(480,110);
		glVertex2i(480,20);
		glVertex2i(320,20);
		// Dark Blue
		glColor3f(0,0,0.2f);
		glVertex2i(325,105);
		glVertex2i(475,105);
		glVertex2i(475,25);
		glVertex2i(325,25);
	glEnd();

	glColor3f(0,0.8f,0);
	PositionText(330,90);
	Print(Font1,"PITCH%25.2f",Aircraft.Pitch);

	PositionText(330,78);
	Print(Font1,"BANK");
	PositionText(366,78);
	Print(Font1,"%25.2f",Aircraft.Bank);

	glBegin(GL_LINES);
		glVertex2i(330,73);
		glVertex2i(470,73);
	glEnd();

	PositionText(330,61);
//	Print(Font1,"ILS                           ACTIVE");

	glBegin(GL_LINES);
		glVertex2i(330,57);
		glVertex2i(470,57);
	glEnd();

	PositionText(330,45);
	Print(Font1,"TAS");
	PositionText(430,45);
	Print(Font1,"%0.0f",Aircraft.TAS);

	PositionText(330,32);
	Print(Font1,"GS");
	PositionText(430,32);
	Print(Font1,"%0.0f",Aircraft.GS);
}

void Aircraft_Panel_Type::DrawAirspeed(void)
{
	glBegin(GL_QUADS);
		// Background
		glColor3f(0.3f,0.3f,0.3f);
		glVertex2i(180,280);
		glVertex2i(300,280);
		glVertex2i(300,200);
		glVertex2i(180,200);
		// Dark Blue
		glColor3f(0,0,0.2f);
		glVertex2i(185,275);
		glVertex2i(295,275);
		glVertex2i(295,205);
		glVertex2i(185,205);
	glEnd();

	glColor3f(0,0.8f,0);

	PositionText(200,250);
	Print(Font2,"%0.0f",Aircraft.EAS);
	PositionText(239,250);
	Print(Font1,"KEAS");

	PositionText(200,220);
	Print(Font1,"%0.2f     Mach",Aircraft.Mach);
}

void Aircraft_Panel_Type::Draw(void)
{
	// Background panel colors
	glBegin(GL_QUADS);
		glColor3f(0.5f,0.5f,0.5f);
		glVertex2i(0,0);
		glVertex2i(0,300);
		glVertex2i(800,300);
		glVertex2i(800,0);

/*		glColor3f(0.6f,0.7f,1);
		glVertex2i(0,300);
		glVertex2i(0,600);
		glVertex2i(800,600);
		glVertex2i(800,300);*/
	glEnd();

	// Attitude Indicator
	DrawAttInd();
	// MFD 1
	DrawMFD1();
	// Airspeed
	DrawAirspeed();
}