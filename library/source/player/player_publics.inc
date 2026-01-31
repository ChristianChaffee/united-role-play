
forward CheckPlayerNotificationsPublic(playerid);
public CheckPlayerNotificationsPublic(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows) return false;

	for(new i, temp_text[256]; i < rows; i++){
		cache_get_value_name(i, "text", temp_text);

		SendClientMessage(playerid, -1, temp_text);
	}

	static const fmt_query[] = "DELETE FROM notifications WHERE account_id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return true;
}