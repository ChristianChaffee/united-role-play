
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnPlayerConnect(playerid){
	SetTimerEx("ShowLogoTimer", 4000, false, "i", playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}