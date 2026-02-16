
stock Fraction:ShowMenuUI(playerid){
	if(!GetPlayerData(playerid, p_fraction)) return SendErrorMessage(playerid, "Вы не состоите во фракции.");

	new fraction_index = GetPlayerFractionIndex(playerid);
	if(fraction_index == -1) return false;

	SetPVarInt(playerid, "Fraction_InMenuUI", 1);

	new fraction_ranks_str[64];
	format(fraction_ranks_str, sizeof(fraction_ranks_str), "%s [%d]",
		fraction_ranks[fraction_index][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_fraction_rank));

	cef_emit_event(playerid, "showFactionMenu",
		CEFSTR(GetFractionData(fraction_index, f_icon_url)),
		CEFSTR(GetFractionData(fraction_index, f_name)),
		CEFSTR(GetPlayerData(playerid, p_name)),
		CEFSTR(fraction_ranks_str),
		CEFINT(GetPlayerData(playerid, p_fraction_hours)),
		CEFSTR(FRACTION_MENU_INFO_URL),
		CEFSTR(FRACTION_MENU_MEMBERS_URL),
		CEFSTR(FRACTION_MENU_HELP_URL));

	return true;
}

stock Fraction:HideMenuUI(playerid){
	DeletePVar(playerid, "Fraction_InMenuUI");
	cef_emit_event(playerid, "hideFactionMenu");
	return true;
}

stock Fraction:ShowMembersUI(playerid){
	if(!GetPlayerData(playerid, p_fraction)) return SendErrorMessage(playerid, "Вы не состоите во фракции.");

	Fraction:HideMenuUI(playerid);

	SetPVarInt(playerid, "Fraction_InMembersUI", 1);
	cef_emit_event(playerid, "showFactionMembers");

	#warning MAKE_AFK
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!IsPlayerConnected(i) && !player_logged[i]) continue;
		if(GetPlayerData(i, p_fraction) != GetPlayerData(playerid, p_fraction)) continue;

		cef_emit_event(playerid, "addFactionOnlineMember",
			CEFSTR(GetPlayerData(i, p_name)),
			CEFINT(i),
			CEFSTR(fraction_ranks[GetPlayerFractionIndex(i)][GetPlayerData(i, p_fraction_rank) - 1]),
			CEFINT(GetPlayerData(i, p_fraction_rank)),
			CEFINT(0),
			CEFINT((GetPlayerData(i, p_fraction_duty)) ? (0) : (1)));
	}
	return true;
}

stock Fraction:HideMembersUI(playerid){
	DeletePVar(playerid, "Fraction_InMembersUI");
	cef_emit_event(playerid, "hideFactionMembers");
	return true;
}