
CMD:hpanel(playerid){
	if(!player_logged[playerid]) return false;

	new house_index = House:GetPlayerHouseIndex(playerid);

	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в личном доме.");
		return false;
	}

	House:ShowMenu(playerid, house_index);
	return true;
}

CMD:leavehouse(playerid){
	if(!player_logged[playerid]) return false;

	if(!GetPlayerData(playerid, p_tenant_house_id)) return SendErrorMessage(playerid, "Вы не живёте в доме.");

	new scdplayerid = GetPlayerIdBySqlId(GetHouseData(GetPlayerData(playerid, p_tenant_house_id), h_owner_id));

	small_string[0] = EOS;
	if(scdplayerid != INVALID_PLAYER_ID){
		format(small_string, sizeof(small_string),
			"Жилец %s[%d] выселился(-ась) из Вашего дома №%d.",
			GetPlayerData(playerid, p_name), playerid,
			GetPlayerData(playerid, p_tenant_house_id));
		SendClientMessage(scdplayerid, COLOR_DARK_RED, small_string);
	}
	else{
		format(small_string, sizeof(small_string),
			"Жилец %s выселился(-ась) из Вашего дома №%d.",
			GetPlayerData(playerid, p_name), GetPlayerData(playerid, p_tenant_house_id));
		AddPlayerNotification(GetHouseData(GetPlayerData(playerid, p_tenant_house_id), h_owner_id), small_string);
	}

	format(small_string, sizeof(small_string),
		"Вы выселились из дома "cl_white"№%d"cl_lime".",
		GetPlayerData(playerid, p_tenant_house_id));
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	SetPlayerData(playerid, p_tenant_house_id, 0);

	static const fmt_query[] = "UPDATE accounts SET tenant_house_id = 0 WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return true;
}