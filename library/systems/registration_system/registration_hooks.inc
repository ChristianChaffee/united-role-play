
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid){
	SetSpawnInfo(playerid, 0, random(311), 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerSpawn(playerid){
	if(!player_logged[playerid]){
		GetPlayerName(playerid, GetPlayerData(playerid, p_name), MAX_PLAYER_NAME);
		static const fmt_query[] = "SELECT * FROM accounts WHERE name = '%s' LIMIT 1";

		new query[sizeof(fmt_query) - 2 + 24];
		mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_name));
		mysql_tquery(mysql, query, "CheckPlayerRegistration", "i", playerid);

		Registration:SetCamera(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook OnPlayerLevelChanged(playerid, old_level, new_level){
	if(GetPlayerStartQuestData(playerid, SQ_THIRD_LEVEL, sqp_status) == SQ_STATUS_IN_PROGRESS && GetPlayerData(playerid, p_level) >= 3){
		StartQuest:UpdateQuest(playerid, SQ_THIRD_LEVEL);
	}
	return continue(playerid, old_level, new_level);
}