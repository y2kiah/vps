/* ----==== VERTICAL PLANE SIMULATOR ====----

	By:			Jeff Kiah
	Started:	1/2003
*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mmsystem.h>
#include "main.h"
#include "aircraft.h"
#include "panel.h"

/*---------------
---- DEFINES ----
---------------*/

#define RESX				800
#define RESY				600
#define FULLSCREEN			1
#define COLORDEPTH			16

#define KeyDown(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/*-----------------
---- VARIABLES ----
-----------------*/

HINSTANCE		hInst;
MSG				Msg;
HWND			hWnd = NULL;
HDC				hDC = NULL;
HGLRC			hRC = NULL;

Sim_Timer_Type			Timer;
Sim_Options_Type		Options = {0,0,0,0,false,false,false,true,1.0f};
Environment_Type		Environment = {0,false,0,0};
Aircraft_Type			Aircraft;
Aircraft_Panel_Type		Panel;

DWORD			fpsTime;
float			fps;
unsigned int	Font1, Font2;
bool			ButtonDown[256];
int				NumFrames = 0;

/*-----------------
---- FUNCTIONS ----
-----------------*/

////////// struct Sim_Timer_Type //////////

void Sim_Timer_Type::GetTimeFix(void)
{
	QueryPerformanceCounter((LARGE_INTEGER *)&CurrentTime);
	TimeFix = (double)(CurrentTime - LastTime) * TimerFrequencyFix;
	LastTime = CurrentTime;
}

Sim_Timer_Type::Sim_Timer_Type()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&TimerFrequency);
	TimerFrequencyFix = 1.0f / (float)TimerFrequency;
}

//////////

void LoadFont(void)
{
	HFONT NewFont;
	Font1 = glGenLists(96);
	Font2 = glGenLists(96);

	NewFont = CreateFont(	-12,							// Height Of Font
							0,								// Width Of Font
							0,								// Angle Of Escapement
							0,								// Orientation Angle
							FW_NORMAL,						// Font Weight
							FALSE,							// Italic
							FALSE,							// Underline
							FALSE,							// Strikeout
							ANSI_CHARSET,					// Character Set Identifier
							OUT_TT_PRECIS,					// Output Precision
							CLIP_DEFAULT_PRECIS,			// Clipping Precision
							DEFAULT_QUALITY,				// Output Quality
							FF_DONTCARE | DEFAULT_PITCH,	// Family And Pitch
							"Arial");						// Font Name

	SelectObject(hDC, NewFont);
	wglUseFontBitmaps(hDC, 32, 96, Font1);
	DeleteObject(NewFont);

	NewFont = CreateFont(	-16,							// Height Of Font
							0,								// Width Of Font
							0,								// Angle Of Escapement
							0,								// Orientation Angle
							FW_NORMAL,						// Font Weight
							FALSE,							// Italic
							FALSE,							// Underline
							FALSE,							// Strikeout
							ANSI_CHARSET,					// Character Set Identifier
							OUT_TT_PRECIS,					// Output Precision
							CLIP_DEFAULT_PRECIS,			// Clipping Precision
							DEFAULT_QUALITY,				// Output Quality
							FF_DONTCARE | DEFAULT_PITCH,	// Family And Pitch
							"Arial");						// Font Name

	SelectObject(hDC, NewFont);
	wglUseFontBitmaps(hDC, 32, 96, Font2);
	DeleteObject(NewFont);
}

void DeleteFont(void)
{
	glDeleteLists(Font1, 96);
	glDeleteLists(Font2, 96);
}

void PositionText(int x, int y)
{
	glPushAttrib(GL_TRANSFORM_BIT | GL_VIEWPORT_BIT);

	glMatrixMode(GL_PROJECTION);				// Set our matrix to our projection matrix
	glPushMatrix();								// Push on a new matrix to work with
	glLoadIdentity();							// reset the matrix
	glMatrixMode(GL_MODELVIEW);					// Set our matrix to our model view matrix
	glPushMatrix();								// Push on a new matrix to work with
	glLoadIdentity();							// Reset that matrix

	glViewport(x, y, 0, 0);						// Create a new viewport to draw into

	glRasterPos4f(0, 0, 0, 1);					// Set the drawing position

	glPopMatrix();								// Pop the current modelview matrix off the stack
	glMatrixMode(GL_PROJECTION);				// Go back into projection mode
	glPopMatrix();								// Pop the projection matrix off the stack

	glPopAttrib();								// This restores our TRANSFORM and VIEWPORT attributes
}

void Print(unsigned int Font, const char *Text, ...)
{
	char TempString[256];

	TempString[0] = NULL;
	va_list	Arg;								// Pointer to list of arguments

	if (Text == NULL) return;

	va_start(Arg, Text);						// Parses the string for variables
	vsprintf(TempString, Text, Arg);			// And converts symbols to actual numbers
	va_end(Arg);								// Results are stored in text line

	glPushAttrib(GL_LIST_BIT);
	
	glListBase(Font - 32);
	glCallLists(strlen(TempString), GL_UNSIGNED_BYTE, TempString);

	glPopAttrib();
}

void InitOpenGL(void)
{
	glViewport(0,0,RESX,RESY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,RESX,0,RESY);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_FLAT);
	glClearColor(0,0,0,1);

	glClear(GL_COLOR_BUFFER_BIT);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
	glHint(GL_FOG_HINT, GL_FASTEST);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_CULL_FACE);
	glDisable(GL_POINT_SMOOTH);	
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_CREATE:
			return 0;
			break;

		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE || HIWORD(wParam)) {
				Options.Active = false;
				ShowCursor(true);
			} else if (!Options.Active && !HIWORD(wParam)) {
				Options.Active = true;
				ShowCursor(false);
			}
			return 0;
			break;

		case WM_SYSCOMMAND:
			switch (wParam) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			return 0;
			break;
	
		case WM_SIZE:
			return 0;
			break;

		case WM_LBUTTONDOWN:
			return 0;
			break;

		case WM_LBUTTONUP:
			return 0;
			break;

		case WM_RBUTTONUP:
			return 0;
			break;

		default:
			return DefWindowProc(hwnd,msg,wParam,lParam);
	}

	return 0;
}

void KillWindow(void) {
	if (FULLSCREEN) {
		ChangeDisplaySettings(NULL,0);
		ShowCursor(true);
	}

	if (hRC) {
		if (!wglMakeCurrent(NULL,NULL))
			MessageBox(0,"Release Of DC And RC Failed","Error",MB_OK | MB_ICONERROR);

		if (!wglDeleteContext(hRC))
			MessageBox(0,"Release Rendering Context Failed","Error",MB_OK | MB_ICONERROR);

		hRC = NULL;
	}

	if (hDC && !ReleaseDC(hWnd,hDC)) {
		MessageBox(0,"Release Device Context Failed","Error",MB_OK | MB_ICONERROR);
		hDC = NULL;
	}

	if (hWnd && !DestroyWindow(hWnd))
	{
		MessageBox(NULL,"Could Not Release hWnd","Error",MB_OK | MB_ICONERROR);
		hWnd = NULL;
	}

	if (!UnregisterClass("Landscape Engine",hInst)) {
		MessageBox(NULL,"Could Not Unregister Class","Error",MB_OK | MB_ICONERROR);
		hInst = NULL;
	}
}

bool InitWindow(void)
{
	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	UINT		PixelFormat;
	
	hInst = GetModuleHandle(NULL);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Vertical Plane Simulator";

	if (!RegisterClass(&wc)) {
		MessageBox(0,"Failed To Register The Window Class","Error",MB_OK | MB_ICONERROR);
		return false;
	}

	if (FULLSCREEN) {
		
		DEVMODE dmScreenSettings;

		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));		
		dmScreenSettings.dmSize	= sizeof(DEVMODE);
		dmScreenSettings.dmPelsWidth = RESX;
		dmScreenSettings.dmPelsHeight = RESY;
		dmScreenSettings.dmBitsPerPel = COLORDEPTH;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		int Check = ChangeDisplaySettings(&dmScreenSettings,CDS_TEST);
		
		switch (Check) {
			case DISP_CHANGE_SUCCESSFUL:
				ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
				break;

			case DISP_CHANGE_FAILED:
				KillWindow();
				MessageBox(0,"Failed To Change To Desired Settings","Error",MB_OK | MB_ICONERROR);
				return false;

			case DISP_CHANGE_BADMODE:
				KillWindow();
				MessageBox(0,"Fullscreen Video Mode Not Supported","Error",MB_OK | MB_ICONERROR);
				return false;

			case DISP_CHANGE_RESTART:
				KillWindow();
				MessageBox(0,"Must Restart To Get Fullscreen Video Mode","Error",MB_OK | MB_ICONERROR);
				return false;
		}
	}

	if (FULLSCREEN) {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
		dwStyle = WS_POPUP;
	} else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_POPUP;//OVERLAPPED;
	}

	hWnd = CreateWindowEx(dwExStyle,"Vertical Plane Simulator","Vertical Plane Simulator",dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,0,0,RESX,RESY,NULL,NULL,hInst,NULL);
	if (!hWnd) {
		KillWindow();
		MessageBox(0,"Window Creation Error","Error",MB_OK | MB_ICONERROR);
		return false;
	}

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		COLORDEPTH,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		0,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	hDC = GetDC(hWnd);
	if (!hDC) {
		KillWindow();
		MessageBox(NULL,"Can't Create A Device Context","Error",MB_OK | MB_ICONERROR);
		return false;
	}

	PixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!PixelFormat) {
		KillWindow();
		MessageBox(0,"Cannot Find Suitable Pixel Format","Error",MB_OK | MB_ICONERROR);
		return false;
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
		KillWindow();
		MessageBox(0,"Cannot Set The Pixel Format","Error",MB_OK | MB_ICONERROR);
		return false;
	}

	hRC = wglCreateContext(hDC);
	if (!hRC) {
		KillWindow();
		MessageBox(0,"Cannot Create An OpenGL Rendering Context","Error",MB_OK | MB_ICONERROR);
		return false;
	}

	if (!wglMakeCurrent(hDC, hRC)) {
		KillWindow();
		MessageBox(0,"Cannot Activate GLRC","Error",MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);
	wglMakeCurrent(hDC, hRC);	

//	ReSizeGLScene(width, height);

	return true;
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int iCmdShow)
{
	for (int c = 0; c < 256; c++) ButtonDown[c] = false;

	if (!InitWindow()) return 0;

	InitOpenGL();
	LoadFont();

	srand(time(0));
	fpsTime = timeGetTime();
	
	Timer.GetTimeFix();

	while (1) {
		if (PeekMessage(&Msg,NULL,0,0,PM_REMOVE)) {
			if (Msg.message == WM_QUIT) break;
			else {
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}

		Timer.GetTimeFix();

		if (Options.Active) {
			// Exit Program
			if (KeyDown(VK_ESCAPE)) SendMessage(hWnd,WM_CLOSE,0,0);

			if (!Options.Paused) {
				
				// Pause Program
				if (KeyDown('Z')) {
					if (!ButtonDown['Z']) {
						ButtonDown['Z'] = true;

						Options.Paused = true;
						glColor3f(0,1,0);
						PositionText(378,450);
						Print(Font1,"PAUSED");
					}
				} else ButtonDown['Z'] = false;

				Aircraft.Input();
				Aircraft.Update();
				Panel.Draw();

				// Print fps
				glColor3f(1,1,1);
				PositionText(5,590);
				Print(Font1,"fps: %0.3f",fps);

				glFlush();
				SwapBuffers(hDC);
				glClear(GL_COLOR_BUFFER_BIT);//TEMP

				///// Get fps /////
				if (NumFrames == 100) {
					fps = 100.0f / ((float)(timeGetTime() - fpsTime) * 0.001f);
					fpsTime = timeGetTime();
					NumFrames = 0;
				}

				NumFrames++;

			} else {
				if (KeyDown('Z')) {
					if (!ButtonDown['Z']) {
						ButtonDown['Z'] = true;

						Options.Paused = false;
					}
				} else ButtonDown['Z'] = false;
			}
		}
	}

	KillWindow();
	DeleteFont();

	return Msg.wParam;
}