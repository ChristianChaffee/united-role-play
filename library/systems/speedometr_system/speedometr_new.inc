
/*
Используемые PVar`ы:
	Speedometr_IsShowed
*/

//==============================================================================
#define Speedometr:%0(%1) 							Spdmtr_%0(%1)

#define SPEEDOMETR_TEXT_COLOR_ON 					-238863617
#define SPEEDOMETR_TEXT_COLOR_OFF 					-2139062017

#define SPEEDOMETR_COLOR_ON 						-238863772
#define SPEEDOMETR_COLOR_OFF 						-1061109660

#define SPEEDOMETR_TIMER 							35

//==============================================================================
new Text:TDEditor_TD[33];
new PlayerText:TDEditor_PTD[MAX_PLAYERS][11];
new speed_string[16];
new health_string[32];