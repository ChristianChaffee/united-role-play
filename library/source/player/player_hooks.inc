
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnPlayerText(playerid, text[]){
	if(!player_logged[playerid]) return Y_HOOKS_CONTINUE_RETURN_0;

	new temp_string[144];
	format(temp_string, sizeof(temp_string),
		"%s[%d]: %s",
		GetPlayerData(playerid, p_name), playerid, text);
	ProxDetector(playerid, 20.0, COLOR_WHITE, temp_string);

	return Y_HOOKS_CONTINUE_RETURN_0;
}