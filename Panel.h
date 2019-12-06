/* ----==== PANEL.H ====---- */

#ifndef PANEL_H
#define PANEL_H

/*------------------
---- STRUCTURES ----
------------------*/

class Aircraft_Type;

class Aircraft_Panel_Type {
	friend Aircraft_Type;

	private:
		void DrawAttInd(void);
		void DrawMFD1(void);
		void DrawAirspeed(void);

	public:
		void Draw(void);

		Aircraft_Panel_Type() {}
		~Aircraft_Panel_Type() {}
};

#endif