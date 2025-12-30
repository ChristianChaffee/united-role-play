
stock PayDay(hour){

	SetWorldTime(hour);

	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		PlayerPayDay(i);
	}
	return true;
}

stock PlayerPayDay(playerid){

	new give_exp = 1;

	//--------------- Выдача
	GivePlayerExp(playerid, give_exp);
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