
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

hook function SetPlayerHealth(playerid, Float:health){
	SetPlayerData(playerid, p_health, health);
	
	static const fmt_query[] = "UPDATE accounts SET health = %f WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, health, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return continue(playerid, health);
}