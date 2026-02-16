
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
	static const fmt_query[] = "UPDATE accounts SET fraction = 0, fraction_rank = 0, fraction_skin = 0, fraction_hours = 0, fraction_gun_cooldown = 0 WHERE id = %d";
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
		SetPlayerData(playerid, p_fraction_gun_cooldown, 0);

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

stock Fraction:UpdateDressroom(fraction_index){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS) return false;

	if(IsValidDynamicPickup(GetFractionData(fraction_index, f_dressroom_pic))){
		DestroyDynamicPickup(GetFractionData(fraction_index, f_dressroom_pic));
	}
	if(IsValidDynamic3DTextLabel(GetFractionData(fraction_index, f_dressroom_text))){
		DestroyDynamic3DTextLabel(GetFractionData(fraction_index, f_dressroom_text));
	}

	if(GetFractionData(fraction_index, f_dressroom_x) == 0.0 && GetFractionData(fraction_index, f_dressroom_y) == 0.0
		&& GetFractionData(fraction_index, f_dressroom_z) == 0.0) return true;

	GetFractionData(fraction_index, f_dressroom_pic) = CreateDynamicPickup(FRAC_DRESSROOM_PICKUP_MODEL, 23, 
		GetFractionData(fraction_index, f_dressroom_x), 
		GetFractionData(fraction_index, f_dressroom_y), 
		GetFractionData(fraction_index, f_dressroom_z), 
		GetFractionData(fraction_index, f_dressroom_vw),
		GetFractionData(fraction_index, f_dressroom_int));

	GetFractionData(fraction_index, f_dressroom_text) = CreateDynamic3DTextLabel(""cl_blue"Раздевалка\n\
		"cl_white"Используйте "cl_grey"'Left ALT'", 
		-1, 
		GetFractionData(fraction_index, f_dressroom_x), 
		GetFractionData(fraction_index, f_dressroom_y), 
		GetFractionData(fraction_index, f_dressroom_z), 
		15.0, _, _, 1, 
		GetFractionData(fraction_index, f_dressroom_vw),
		GetFractionData(fraction_index, f_dressroom_int));

	return true;
}

stock Fraction:UpdateGuns(fraction_index){
	if(IsValidDynamicPickup(GetFractionData(fraction_index, f_gun_pic))){
		DestroyDynamicPickup(GetFractionData(fraction_index, f_gun_pic));
	}
	if(IsValidDynamic3DTextLabel(GetFractionData(fraction_index, f_gun_text))){
		DestroyDynamic3DTextLabel(GetFractionData(fraction_index, f_gun_text));
	}

	if(GetFractionData(fraction_index, f_gun_x) == 0.0 && GetFractionData(fraction_index, f_gun_y) == 0.0
		&& GetFractionData(fraction_index, f_gun_z) == 0.0) return true;

	GetFractionData(fraction_index, f_gun_pic) = CreateDynamicPickup(FRAC_GUN_PICKUP_MODEL, 23, 
		GetFractionData(fraction_index, f_gun_x), 
		GetFractionData(fraction_index, f_gun_y), 
		GetFractionData(fraction_index, f_gun_z), 
		GetFractionData(fraction_index, f_gun_vw),
		GetFractionData(fraction_index, f_gun_int));

	GetFractionData(fraction_index, f_gun_text) = CreateDynamic3DTextLabel(""cl_blue"Оружейная\n\
		"cl_white"Используйте "cl_grey"'Left ALT'", 
		-1, 
		GetFractionData(fraction_index, f_gun_x), 
		GetFractionData(fraction_index, f_gun_y), 
		GetFractionData(fraction_index, f_gun_z), 
		15.0, _, _, 1, 
		GetFractionData(fraction_index, f_gun_vw),
		GetFractionData(fraction_index, f_gun_int));

	return true;
}

stock Fraction:GiveFracWeapon(playerid){
	new fraction_index = GetPlayerFractionIndex(playerid);
	if(fraction_index == -1) return false;
	if(GetPlayerData(playerid, p_fraction_gun_cooldown) > 0){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Взять комплект вооружения можно через %d секунд(-ы).",
			GetPlayerData(playerid, p_fraction_gun_cooldown));
		SendErrorMessage(playerid, small_string);
		small_string[0] = EOS;
		return true;
	}

	big_string[0] = EOS;

	format(big_string, sizeof(big_string),
		""cl_blue"Вы получили с оружейной:");

	new count;
	for(new i; i < MAX_FRACTION_GUNS; i++){
		if(!fraction_gun_info[GetFractionData(fraction_index, f_type)][i][f_gun_id]) continue;
		if(fraction_gun_info[GetFractionData(fraction_index, f_type)][i][f_gun_rank] > GetPlayerData(playerid, p_fraction_rank)) continue;

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_white"- %s "cl_grey"(%d)",
			big_string, 
			weapon_config[GetWeaponConfigIndex(fraction_gun_info[GetFractionData(fraction_index, f_type)][i][f_gun_id])][w_name], 
			fraction_gun_info[GetFractionData(fraction_index, f_type)][i][f_gun_ammo]);


		GivePlayerWeapon(playerid, fraction_gun_info[GetFractionData(fraction_index, f_type)][i][f_gun_id], fraction_gun_info[GetFractionData(fraction_index, f_type)][i][f_gun_ammo]);
		count++;
	}

	if(!count){
		SendErrorMessage(playerid, "Для Вас нет воружения.");
	}
	else{
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"[R] %s %s[%d] взял(-а) комплект вооружения.",
			fraction_ranks[fraction_index][GetPlayerData(playerid, p_fraction_rank) - 1],
			GetPlayerData(playerid, p_name), playerid);
		Fraction:SendMessage(fraction_index, COLOR_GREEN, small_string);
		small_string[0] = EOS;

		Dialog_Open(playerid, Dialog:D_NULL, DIALOG_STYLE_MSGBOX,
			"Оружейная", big_string,
			"Закрыть", "");

		SetPlayerData(playerid, p_fraction_gun_cooldown, FRAC_GUN_COOLDOWN);
	}

	SetPlayerArmour(playerid, 100.0);
	SetPlayerHealth(playerid, 100.0);
	big_string[0] = EOS;
	return true;
}