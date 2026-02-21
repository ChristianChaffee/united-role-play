
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
	if(IsPlayerAFK(playerid)){
		player_afk[playerid]++;
	}
	if(!IsPlayerAFK(playerid) && player_afk[playerid] > 0){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Вы простояли в AFK %d секунд(-ы).",
			player_afk[playerid]);
		SendClientMessage(playerid, COLOR_GREEN, small_string);
		small_string[0] = EOS;

		player_afk[playerid] = 0;
	}
	if(GetPVarType(playerid, "Player_FreezeTimer") && GetPVarInt(playerid, "Player_FreezeTimer") > 0){
		SetPVarInt(playerid, "Player_FreezeTimer", GetPVarInt(playerid, "Player_FreezeTimer") - 1);
		if(GetPVarInt(playerid, "Player_FreezeTimer") <= 0){
			DeletePVar(playerid, "Player_FreezeTimer");
			TogglePlayerControllable(playerid, true);
		}
	}

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