
stock PlayerNeeds:ClearValues(playerid){
	for(new i; i < PLAYER_NEEDS_COUNT; i++) { SetPlayerNeedValue(playerid, i, PLAYER_NEED_MAX_VALUE); }
}

stock PlayerNeeds:SaveNeeds(playerid){
	if(!player_logged[playerid]) return false;

	new temp_string[128];
	for(new i; i < PLAYER_NEEDS_COUNT; i++){
		format(temp_string, sizeof(temp_string), "%s%d%s",
			temp_string, GetPlayerNeedValue(playerid, i),
			(i == PLAYER_NEEDS_COUNT - 1) ? ("") : (", "));
	}

	static const fmt_query[] = "UPDATE accounts SET needs = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + sizeof(temp_string)) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, temp_string, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return true;
}

//==============================================================================
stock PlayerNeeds:ShowNeedMenu(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Нужда\t"cl_grey"Значение");

	for(new i; i < PLAYER_NEEDS_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_lime"%d. "cl_white"%s\t%s%d единиц",
			big_string, i + 1,
			GetNeedData(i, n_name),
			(GetPlayerNeedValue(playerid, i) <= GetNeedData(i, n_critical_value)) ? (""cl_light_red"") : (""cl_green""),
			GetPlayerNeedValue(playerid, i));
	}

	Dialog_Open(playerid, Dialog:D_NEEDS_MENU, DIALOG_STYLE_TABLIST_HEADERS,
		"Ваши нужды", big_string,
		"Выбрать", "Закрыть");
	big_string[0] = EOS;
	return true;
}