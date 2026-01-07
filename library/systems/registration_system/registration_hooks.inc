
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid){
	SetSpawnInfo(playerid, 0, random(311), 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerSpawn(playerid){
	if(!player_logged[playerid]){
		Registration:SetCamera(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit(){
	cef_subscribe("Registration_SendPassword", "Registration_SendPassword");
	cef_subscribe("Registration_SendEmail", "Registration_SendEmail");
	cef_subscribe("Registration_SendNationality", "Registration_SendNationality");
	cef_subscribe("Registration_SendAge", "Registration_SendAge");
	cef_subscribe("Registration_Next", "Registration_Next");
	cef_subscribe("Registration_Exit", "Registration_Exit");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook OnPlayerLevelChanged(playerid, old_level, new_level){
	if(GetPlayerStartQuestData(playerid, SQ_THIRD_LEVEL, sqp_status) == SQ_STATUS_IN_PROGRESS && GetPlayerData(playerid, p_level) >= 3){
		StartQuest:UpdateQuest(playerid, SQ_THIRD_LEVEL);
	}
	return continue(playerid, old_level, new_level);
}