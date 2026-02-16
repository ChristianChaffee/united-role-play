
stock Fraction:UpdateStr(fraction_index, row[], value[]){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS) return false;

	static const fmt_query[] = "UPDATE fractions SET `%s` = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 128) + (-2 + 128) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, row, value, GetFractionData(fraction_index, f_id));
	mysql_tquery(mysql, query);
	return true;
}

stock Fraction:UpdateInt(fraction_index, row[], value){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS) return false;

	static const fmt_query[] = "UPDATE fractions SET `%s` = '%d' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 128) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query, row, value, GetFractionData(fraction_index, f_id));
	mysql_tquery(mysql, query);
	return true;
}

stock Fraction:UninvitePlayer(account_id){
	static const fmt_query[] = "UPDATE accounts SET fraction = 0, fraction_rank = 0, fraction_skin = 0, fraction_hours = 0 WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, account_id);
	mysql_tquery(mysql, query);

	for(new i; i < MAX_FRACTIONS; i++){
		if(!GetFractionData(i, f_id)) continue;
		if(!GetFractionData(i, f_leader_account_id)) continue;
		if(GetFractionData(i, f_leader_account_id) != account_id) continue;

		SetFractionData(i, f_leader_account_id, 0);
		Fraction:UpdateInt(i, "leader_account_id", 0);
	}

	if(GetPlayerIdBySqlId(account_id) != INVALID_PLAYER_ID){
		new playerid = GetPlayerIdBySqlId(account_id);

		SetPlayerData(playerid, p_fraction, 0);
		SetPlayerData(playerid, p_fraction_rank, 0);
		SetPlayerData(playerid, p_fraction_skin, 0);
		SetPlayerData(playerid, p_fraction_hours, 0);
		SetPlayerData(playerid, p_fraction_duty, false);
	
		SetPlayerSkin(playerid, GetPlayerData(playerid, p_skin));
		SetPlayerColor(playerid, GetPlayerColorEx(playerid));

		if(GetPVarType(playerid, "Fraction_InMenuUI")){
			Fraction:HideMenuUI(playerid);
		}
		if(GetPVarType(playerid, "Fraction_InMembersUI")){
			Fraction:HideMembersUI(playerid);
		}
	}
	return true;
}

stock Fraction:UpdatePlayerSkinIndex(playerid){
	if(!GetPlayerData(playerid, p_fraction)) return -1;

	new fraction_skin_index = -1;

	for(new i; i < MAX_FRACTION_SKINS; i++){
		if(!GetFractionSkinData(i, fs_id)) continue;
		if(GetFractionSkinData(i, fs_fraction_id) != GetPlayerData(playerid, p_fraction)) continue;
		if(GetFractionSkinData(i, fs_sex) != GetPlayerData(playerid, p_sex)) continue;
		if(GetFractionSkinData(i, fs_rank) > GetPlayerData(playerid, p_fraction_rank)) continue;

		fraction_skin_index = i;
		if(GetFractionSkinData(i, fs_rank) == GetPlayerData(playerid, p_fraction_rank)) break;
	}

	return fraction_skin_index;
}

stock Fraction:GetPlayerSkinIndex(playerid){
	if(!GetPlayerData(playerid, p_fraction)) return -1;
	if(!GetPlayerData(playerid, p_fraction_skin)) return -1;

	for(new i; i < MAX_FRACTION_SKINS; i++){
		if(!GetFractionSkinData(i, fs_id)) continue;
		if(GetFractionSkinData(i, fs_id) != GetPlayerData(playerid, p_fraction_skin)) continue;
		return i;
	}
	return -1;
}

stock Fraction:SendMessage(fraction_index, color, const message[], duty_check = false){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS) return false;
	if(!GetFractionData(fraction_index, f_id)) return false;

	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!IsPlayerConnected(i) || !player_logged[i]) continue;
		if(!GetPlayerData(i, p_fraction)) continue;
		if(GetPlayerData(i, p_fraction) != GetFractionData(fraction_index, f_id)) continue;
		if(duty_check && !GetPlayerData(i, p_fraction_duty)) continue;

		SendClientMessage(i, color, message);
	}
	return true;
}