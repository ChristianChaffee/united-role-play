
stock PayDay(hour){

	SetWorldTime(hour);
	SetWeather(wather_list[random(sizeof(wather_list))]);

	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		PlayerPayDay(i);
	}

	switch(hour){
		case 5:{
			GameTextForAll("~r~RESTART", 5000, 0);
			SendRconCommand("gmx");
		}
	}
	return true;
}

stock PlayerPayDay(playerid){

	new give_exp = 1;

	//--------------- Выдача
	GivePlayerExp(playerid, give_exp);

	if(GetPlayerData(playerid, p_wanted) > 0){
		AddPlayerData(playerid, p_wanted, -1);
		SetPlayerWantedLevel(playerid, GetPlayerData(playerid, p_wanted));

		static const fmt_query[] = "UPDATE accounts SET wanted = %d WHERE id = %d";
		new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetPlayerData(playerid, p_wanted), GetPlayerData(playerid, p_id));
		mysql_tquery(mysql, query);
	}
	//----------------------

	//--------------- Информирование
	SendClientMessage(playerid, -1, ""cl_lime"=================== [ "cl_white"PAY DAY "cl_lime"] ===================");

	format(payday_string, sizeof(payday_string),
		""cl_lime"Получено опыта: "cl_white"+%d (%d/%d)",
		give_exp, GetPlayerData(playerid, p_exp), GetExpToNextLevel(GetPlayerData(playerid, p_level) + 1));
	SendClientMessage(playerid, COLOR_LIME, payday_string);

	SendClientMessage(playerid, -1, ""cl_lime"================================================");
	//------------------------------
}