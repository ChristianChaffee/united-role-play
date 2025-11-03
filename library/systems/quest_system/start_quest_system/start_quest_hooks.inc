
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM start_quests", "LoadStartQuests");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterRCP(playerid){
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnPlayerAccountLoaded(playerid){
	StartQuest:LoadPlayerStartQuests(playerid);
	return continue (playerid);
}