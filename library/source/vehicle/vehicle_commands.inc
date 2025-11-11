
CMD:cars(playerid){
	if(!player_logged[playerid]) return false;

	static const fmt_query[] = "SELECT id, model_id FROM vehicles WHERE owner = %d AND status = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id), VEHICLE_STATUS_PLAYER);
	mysql_tquery(mysql, query, "ShowPlayerCars", "i", playerid);
	return true;
}