
stock Phone:ShowPhone(playerid){
	new is_player_have_bank_card = (!GetPlayerBankCardData(playerid, pbc_id)) ? (false) : (true);

	if(!GetPVarType(playerid, "Phone_Show")){
		cef_emit_event(playerid, "showPhone",
			CEFINT(is_player_have_bank_card));
	}
	else{
		cef_focus_browser(playerid, CEF_BROWSER_LOGO, true);
	}

	if(is_player_have_bank_card){
		cef_emit_event(playerid, "setPhoneBankData",
			CEFINT(GetPlayerBankCardData(playerid, pbc_balance)));
	}

	for(new i, counter; i < MAX_BUSINESS; i++){
		if(!GetBusinessData(i, b_id)) continue;
		if(GetBusinessData(i, b_owner_id) != GetPlayerData(playerid, p_id)) continue;
		
		cef_emit_event(playerid, "addPhoneBusiness",
			CEFSTR(GetBusinessData(i, b_name)),
			CEFINT(GetBusinessData(i, b_balance)));

		player_listitems[playerid][counter] = i;
		counter++;
	}

	cef_emit_event(playerid, "clearPhoneBankHouses");

	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(playerid, p_id)) continue;

		cef_emit_event(playerid, "addPhoneBankHouse",
			CEFINT(i),
			CEFINT(GetHouseData(i, h_id)),
			CEFINT(GetHouseData(i, h_payment_days)),
			CEFINT(house_class_payment_price[GetHouseData(i, h_class)]));
	}

	static const fmt_query[] = "SELECT id, model_id FROM vehicles WHERE owner = %d AND status = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id), VEHICLE_STATUS_PLAYER);
	mysql_tquery(mysql, query, "ShowPlayerCars", "i", playerid);

	SetPVarInt(playerid, "Phone_Show", 1);
	return true;
}

stock Phone:HidePhone(playerid){
	cef_emit_event(playerid, "hidePhone");
	DeletePVar(playerid, "Phone_Show");
	return true;
}

stock Phone:ShowError(playerid, text[]){
	cef_emit_event(playerid, "showPhoneError",
		CEFSTR(text));
	return true;
}

stock Phone:StopCall(playerid){
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	Phone:HidePhone(playerid);

	DeletePVar(playerid, "Phone_CallID");
	return true;
}