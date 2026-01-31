
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnPlayerDisconnect(playerid, reason){
	static const fmt_query[] = "UPDATE accounts SET drunked_level = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerDrunkLevel(playerid), GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);

	UpdatePlayerWeapons(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerText(playerid, text[]){
	if(!player_logged[playerid]) return Y_HOOKS_CONTINUE_RETURN_0;

	if(GetPVarType(playerid, "Phone_CallID") && GetPlayerCallerId(playerid) != INVALID_PLAYER_ID){
		new temp_string[144];
		format(temp_string, sizeof(temp_string),
			"[Телефон] %s[%d]: %s",
			GetPlayerData(playerid, p_name), playerid, text);
		
		ProxDetector(playerid, 20.0, COLOR_GREY, temp_string);
		ProxDetector(GetPlayerCallerId(playerid), 20.0, COLOR_GREY, temp_string);
	}
	else{
		new temp_string[144];
		format(temp_string, sizeof(temp_string),
			"%s[%d]: %s",
			GetPlayerData(playerid, p_name), playerid, text);
		ProxDetector(playerid, 20.0, COLOR_WHITE, temp_string);
	}

	return Y_HOOKS_CONTINUE_RETURN_0;
}

//==============================================================================
hook function SetPlayerHealth(playerid, Float:health){
	SetPlayerData(playerid, p_health, health);
	
	static const fmt_query[] = "UPDATE accounts SET health = %f WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, health, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return continue(playerid, health);
}

hook function SetPlayerDrunkLevel(playerid, level){
	if(level > 50000) level = 50000;
	if(level < 0) level = 0;

	SetPlayerData(playerid, p_drunked_level, level);
	continue (playerid, level);
}

//==============================================================================
hook stock OnPlayerSecondUpdate(playerid){
	if(GetPVarType(playerid, "Mask") && GetPVarType(playerid, "Mask_Timer") && GetPVarInt(playerid, "Mask_Timer") > 0){
		SetPVarInt(playerid, "Mask_Timer", GetPVarInt(playerid, "Mask_Timer") - 1);
		if(GetPVarInt(playerid, "Mask_Timer") <= 0){
			DeletePVar(playerid, "Mask");
			DeletePVar(playerid, "Mask_Timer");
		
			SetPlayerColor(playerid, GetPlayerColorEx(playerid));
			GameTextForPlayer(playerid, "~b~Invisible ~r~OFF", 5000, 3);
		}
	}
	return continue(playerid);
}