
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	SetTimer("HudTimerUpdate", HUD_UPDATE_TIME, true);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	player_hud_info[playerid] = player_hud_info_clear;
	return Y_HOOKS_CONTINUE_RETURN_1;
}