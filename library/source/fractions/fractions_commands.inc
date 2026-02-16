
CMD:fmenu(playerid){
	if(!player_logged[playerid]) return false;
	Fraction:ShowMenuUI(playerid);
	return true;
}

CMD:r(playerid, params[]){
	if(!player_logged[playerid]) return false;
	if(sscanf(params, "s[128]", params[0])) return SendErrorMessage(playerid, "Используйте '/r [Текст]'");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[R] %s %s[%d]: %s",
		fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_name), playerid, params[0]);
	Fraction:SendMessage(GetPlayerFractionIndex(playerid), 0x9A72DBFF, small_string, true);
	small_string[0] = EOS;
	return true;
}

CMD:rb(playerid, params[]){
	if(!player_logged[playerid]) return false;
	if(sscanf(params, "s[128]", params[0])) return SendErrorMessage(playerid, "Используйте '/rb [Текст]'");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"(( [R] %s %s[%d]: %s ))",
		fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_name), playerid, params[0]);
	Fraction:SendMessage(GetPlayerFractionIndex(playerid), 0x9A72DBFF, small_string, true);
	small_string[0] = EOS;
	return true;
}

CMD:members(playerid, params[]){
	if(!player_logged[playerid]) return false;
	Fraction:ShowMembersUI(playerid);
	return true;
}