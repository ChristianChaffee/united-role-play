
CMD:taxes(playerid){
	if(!player_logged[playerid]) return false;
	Taxes:ShowMainMenu(playerid);
	return true;
}

CMD:mytaxes(playerid){
	if(!player_logged[playerid]) return false;

	static const fmt_query[] = "SELECT * FROM player_taxes WHERE account_id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query, "ShowPlayerTaxes", "i", playerid);
	return true;
}