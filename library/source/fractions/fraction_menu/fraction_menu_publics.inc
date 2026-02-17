
forward FactionMenu_Members(playerid, const arguments[]);
public FactionMenu_Members(playerid, const arguments[]){
	Fraction:ShowMembersUI(playerid);
	return true;
}

forward FactionMembers_Close(playerid, const arguments[]);
public FactionMembers_Close(playerid, const arguments[]){
	Fraction:HideMembersUI(playerid);
	return true;
}

forward FactionMember_ChangeRank(playerid, const arguments[]);
public FactionMember_ChangeRank(playerid, const arguments[]){
	new scdplayerid, rank;
	sscanf(arguments, "ii", scdplayerid, rank);

	if(GetPlayerData(playerid, p_fraction_rank) < GetFractionData(GetPlayerFractionIndex(playerid), f_ranks_count) - 1){
		SendErrorMessage(playerid, "У Вас нет доступа к изменению ранга.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(!IsPlayerConnected(scdplayerid) || !player_logged[scdplayerid]){
		SendErrorMessage(playerid, "Данный игрок не в сети.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(GetPlayerData(scdplayerid, p_fraction) != GetPlayerData(playerid, p_fraction)){
		SendErrorMessage(playerid, "Данный игрок не состоит в Вашей фракции.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(scdplayerid == playerid){
		SendErrorMessage(playerid, "Вы не можете изменить ранг самому себе.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(GetPlayerData(scdplayerid, p_fraction_rank) >= GetPlayerData(playerid, p_fraction_rank)){
		SendErrorMessage(playerid, "Вы не можете изменить ранг данному игроку.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(rank >= GetPlayerData(playerid, p_fraction_rank)){
		SendErrorMessage(playerid, "Вы не можете выдать данный ранг этому игроку.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(rank < 0 || rank >= GetFractionData(GetPlayerFractionIndex(playerid), f_ranks_count)){
		SendErrorMessage(playerid, "Вы некорректно ввели ранг.");
		Fraction:HideMembersUI(playerid);
		return false;
	}

	SetPlayerData(scdplayerid, p_fraction_rank, rank);
	new fraction_skin_index = Fraction:UpdatePlayerSkinIndex(scdplayerid);
	if(fraction_skin_index != -1){
		SetPlayerData(scdplayerid, p_fraction_skin, GetFractionSkinData(fraction_skin_index, fs_id));	
	}

	if(GetPlayerData(scdplayerid, p_fraction_duty)){
		SetPlayerSkin(scdplayerid, GetFractionSkinData(fraction_skin_index, fs_skin_id));
	}

	static const fmt_query[] = "UPDATE accounts SET fraction_rank = %d, fraction_skin = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 3)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(scdplayerid, p_fraction_rank), GetPlayerData(scdplayerid, p_fraction_skin),
		GetPlayerData(scdplayerid, p_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы изменили ранг %s[%d] на \"%s\" (%d).",
		GetPlayerData(scdplayerid, p_name), scdplayerid,
		fraction_ranks[GetPlayerFractionIndex(playerid)][rank - 1], rank);
	SendClientMessage(playerid, COLOR_BLUE, small_string);

	format(small_string, sizeof(small_string),
		"%s %s[%d] изменил(-а) Вам ранг на \"%s\" (%d).",
		fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_name), playerid,
		fraction_ranks[GetPlayerFractionIndex(playerid)][rank - 1], rank);
	SendClientMessage(scdplayerid, COLOR_BLUE, small_string);
	small_string[0] = EOS;

	Fraction:HideMembersUI(playerid);
	return true;
}

forward FactionMember_Fire(playerid, const arguments[]);
public FactionMember_Fire(playerid, const arguments[]){
	new scdplayerid, reason[32];
	sscanf(arguments, "is[32]", scdplayerid, reason);

	if(GetPlayerData(playerid, p_fraction_rank) < GetFractionData(GetPlayerFractionIndex(playerid), f_ranks_count) - 1){
		SendErrorMessage(playerid, "У Вас нет доступа к увольнению.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(!IsPlayerConnected(scdplayerid) || !player_logged[scdplayerid]){
		SendErrorMessage(playerid, "Данный игрок не в сети.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(GetPlayerData(scdplayerid, p_fraction) != GetPlayerData(playerid, p_fraction)){
		SendErrorMessage(playerid, "Данный игрок не состоит в Вашей фракции.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(scdplayerid == playerid){
		SendErrorMessage(playerid, "Вы не можете уволить самого себя.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	if(GetPlayerData(scdplayerid, p_fraction_rank) >= GetPlayerData(playerid, p_fraction_rank)){
		SendErrorMessage(playerid, "Вы не можете уволить данного игрока.");
		Fraction:HideMembersUI(playerid);
		return false;
	}
	
	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[R] %s %s[%d] уволил(-а) %s %s[%d] по причине: %s",
		fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_name), playerid,
		fraction_ranks[GetPlayerFractionIndex(scdplayerid)][GetPlayerData(scdplayerid, p_fraction_rank) - 1],
		GetPlayerData(scdplayerid, p_name), scdplayerid, reason);

	Fraction:SendMessage(GetPlayerFractionIndex(playerid), COLOR_LIGHT_RED, small_string);
	small_string[0] = EOS;

	Fraction:UninvitePlayer(GetPlayerData(scdplayerid, p_id));
	Fraction:HideMembersUI(playerid);
	return true;
}

forward FactionMenu_Info(playerid, const arguments[]);
public FactionMenu_Info(playerid, const arguments[]){
	new fraction_index = GetPlayerFractionIndex(playerid);
	if(fraction_index == -1){
		Fraction:HideMenuUI(playerid);
		return false;
	}

	if(!GetFractionData(fraction_index, f_leader_account_id)){
		Fraction:ShowInfoUI(playerid, "Отсутствует");
	}
	else{
		static const fmt_query[] = "SELECT a.name AS leader_name FROM fractions f JOIN accounts a ON a.id = f.leader_account_id WHERE f.id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, GetFractionData(fraction_index, f_id));
		mysql_tquery(mysql, query, "FactionMenu_InfoLoad", "i", playerid);
	}
	return true;
}

forward FactionMenu_InfoLoad(playerid);
public FactionMenu_InfoLoad(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows) return SendErrorMessage(playerid, "Произошла ошибка.");

	new leader_name[MAX_PLAYER_NAME];
	cache_get_value_name(0, "leader_name", leader_name);
	Fraction:ShowInfoUI(playerid, leader_name);
	return true;
}

forward FactionInfo_Close(playerid, const arguments[]);
public FactionInfo_Close(playerid, const arguments[]){
	Fraction:HideInfoUI(playerid);
	return true;
}

forward FactionMenu_Close(playerid, const arguments[]);
public FactionMenu_Close(playerid, const arguments[]){
	Fraction:HideMenuUI(playerid);
	return true;
}