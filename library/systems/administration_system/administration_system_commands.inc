
CMD:alogin(playerid){
	if(!player_logged[playerid]) return false;
	if(!GetPlayerAdminData(playerid, pa_id)) return false;

	if(GetPlayerAdminData(playerid, pa_logged)){
		new temp_ip[32];
		GetPlayerIp(playerid, temp_ip, sizeof(temp_ip));

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"[A] Администратор %d-го уровня %s[%d] вышел из админ-панели (IP: %s).",
			GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid, temp_ip);
		Admin:SendAdminMessage(COLOR_GREY, small_string);
		small_string[0] = EOS;

		SetPlayerAdminData(playerid, pa_logged, false);
	}
	else{
		static const fmt_query[] = "SELECT id FROM administration WHERE password IS NULL AND id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerAdminData(playerid, pa_id));
		mysql_tquery(mysql, query, "IsAdministrationHavePassword", "i", playerid);
	}
	return true;
}

CMD:a(playerid, params[]){
	if(sscanf(params, "s[144]", params[0])) return SendErrorMessage(playerid, "Используйте '/a [Текст]'.");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d]: %s",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid, params[0]);
	Admin:SendAdminMessage(COLOR_LIME, small_string);
	small_string[0] = EOS;
	return true;
}

CMD:kick(playerid, params[]){
	if(sscanf(params, "is[32]", params[0], params[1])) return SendErrorMessage(playerid, "Используйте '/kick [ID] [Причина]'.");

	if(!IsPlayerConnected(params[0])) return SendErrorMessage(playerid, "Данный игрок не в сети.");
	//if(playerid == params[0]) return SendErrorMessage(playerid, "Вы не можете отключить самого себя.");
	if(GetPlayerAdminData(playerid, pa_level) < GetPlayerAdminData(params[0], pa_level)) return SendErrorMessage(playerid, "Вы не можете отключить данного игрока.");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Администратор %s отключил(-а) игрока %s.",
		GetPlayerData(playerid, p_name), GetPlayerData(params[0], p_name));
	Admin:SendAdminActionMessage(COLOR_LIGHT_RED, small_string);

	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d] отключил(-а) игрока %s по причине: %s",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid,
		GetPlayerData(params[0], p_name), params[1]);
	Admin:SendAdminMessage(COLOR_GREY, small_string);

	format(small_string, sizeof(small_string),
		"Администратор %s отключил(-а) Вас от сервера по причине: %s",
		GetPlayerData(playerid, p_name), params[1]);
	SendClientMessage(params[0], COLOR_LIGHT_RED, small_string);
	small_string[0] = EOS;

	KickPlayer(params[0]);
	return true;
}

CMD:ooc(playerid, params[]){
	if(sscanf(params, "s[144]", params[0])) return SendErrorMessage(playerid, "Используйте '/ooc [Текст]'.");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"<< %s[%d]: %s >>",
		GetPlayerData(playerid, p_name), playerid, params[0]);
	SendClientMessageToAll(COLOR_WHITE, small_string);
	small_string[0] = EOS;
	return true;
}