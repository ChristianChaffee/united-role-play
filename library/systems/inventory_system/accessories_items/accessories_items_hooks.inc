
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM accessories_items", "LoadAccessoriesItems");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook LoadPlayerInventoryItems(playerid){
	static const fmt_query[] = "SELECT * FROM player_accessoires p JOIN accounts a ON p.account_id = a.id WHERE a.name = '%s'";
	new query[sizeof(fmt_query) + (-2 + MAX_PLAYER_NAME)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_name));
	mysql_tquery(mysql, query, "LoadPlayerAccessoriesItems", "i", playerid);
}

hook OnPlayerSpawn(playerid){
	if(player_logged[playerid]) Inventory:UpdatePlayersAccessories(playerid);
}