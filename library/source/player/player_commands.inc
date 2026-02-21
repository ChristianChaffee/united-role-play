
CMD:mm(playerid){
	if(!player_logged[playerid]) return false;
	ShowMainMenu(playerid);
	return true;
}

CMD:help(playerid){
	if(!player_logged[playerid]) return false;
	ShowHelpMenu(playerid);
	return true;
}

CMD:maskoff(playerid){
	if(!player_logged[playerid]) return false;
	if(!GetPVarType(playerid, "Mask")) return SendErrorMessage(playerid, "На Вас нет маски.");

	SendMeAction(playerid, "снял(-а) маску.", true);
	DeletePVar(playerid, "Mask");
	DeletePVar(playerid, "Mask_Timer");

	SetPlayerColor(playerid, GetPlayerColorEx(playerid));
	GameTextForPlayer(playerid, "~b~Invisible ~r~OFF", 5000, 3);
	return true;
}

CMD:me(playerid, params[]){
	if(!player_logged[playerid]) return false;

	if(sscanf(params, "s[128]", params[0])) return SendErrorMessage(playerid, "Используйте '/me [действие]'");

	SendMeAction(playerid, params[0]);
	return true;
}

CMD:do(playerid, params[]){
	if(!player_logged[playerid]) return false;

	if(sscanf(params, "s[128]", params[0])) return SendErrorMessage(playerid, "Используйте '/do [текст]'");

	SendDoAction(playerid, params[0]);
	return true;
}

CMD:try(playerid, params[]){
	if(!player_logged[playerid]) return false;

	if(sscanf(params, "s[128]", params[0])) return SendErrorMessage(playerid, "Используйте '/try [текст]'");

	new rnd = random(2);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"%s %s [%s"cl_action"]",
		GetPlayerData(playerid, p_name), params[0],
		(rnd) ? (""cl_green"Удачно") : (""cl_light_red"Неудачно"));

	ProxDetector(playerid, 15.0, COLOR_ACTION, small_string);
	small_string[0] = EOS;
	return true;
}

CMD:time(playerid){
	if(!player_logged[playerid]) return false;

	new temp_year, temp_month, temp_day, temp_hour, temp_minute, temp_second;
	ConvertTime(gettime(), temp_year, temp_month, temp_day, temp_hour, temp_minute, temp_second);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"~w~%02d:%02d:%02d~n~\
		~b~%02d.%02d.%d",
		temp_hour, temp_minute, temp_second, temp_day, temp_month, temp_year);

	GameTextForPlayer(playerid, small_string, 8000, 1);
	small_string[0] = EOS;

	ApplyAnimation(playerid, "COP_AMBIENT", "Coplook_watch", 4.1, 0, 0, 0, 0, 2000, 1);
	return true;
}

CMD:q(playerid){
	if(!IsPlayerConnected(playerid)) return false;

	QuitGameForPlayer(playerid);
	return true;
}