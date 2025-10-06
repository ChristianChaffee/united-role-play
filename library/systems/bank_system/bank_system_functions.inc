
stock Bank:ShowMainMenu(playerid){
	if(GetPlayerBankCardData(playerid, pbc_is_loaded) && !GetPlayerBankCardData(playerid, pbc_id)){

		Dialog_Open(playerid, Dialog:D_BANK_CARD_ACCEPT_CREATE, DIALOG_STYLE_MSGBOX,
			"Банк",
			""cl_blue"- "cl_white"У Вас нет банковской карты.\n\
			\\c"cl_grey"["cl_white"Вы хотите приобрести банковскую карту за "cl_green""#BANK_CARD_CREATE_PRICE"$"cl_white"?"cl_grey"]\n\n\
			\\c"cl_grey"Время ожидания: "cl_white""#BANK_CARD_CREATE_TIME" минут.",
			"Да", "Нет");
		return true;
	}
	if(GetPlayerBankCardData(playerid, pbc_is_loaded) && GetPlayerBankCardData(playerid, pbc_id)
		&& GetPlayerBankCardData(playerid, pbc_status) == BC_STATUS_WAIT){

		if(GetPlayerBankCardData(playerid, pbc_timer) > 0){
			SendErrorMessage(playerid, "Ваша банковская карта ещё не готова.");
			return false;
		}

		new slot_index = Inventory:GetFreeSlotIndex(playerid);
		if(slot_index == -1){
			SendErrorMessage(playerid, "У Вас нет места в инвентаре для получения банковской карты.");
			return false;
		}
		Inventory:GiveItem(playerid, slot_index, ITEM_BANK_CARD, _, GetPlayerBankCardData(playerid, pbc_id), _, _, 0);

		SetPlayerBankCardData(playerid, pbc_pin_code, RandomEx(1000, 9999));
		SetPlayerBankCardData(playerid, pbc_create_time, gettime());
		SetPlayerBankCardData(playerid, pbc_status, BC_STATUS_ACTIVE);

		static const fmt_query[] = "UPDATE bank_cards SET pin_code = %d, create_time = %d, status = %d WHERE id = %d";
		new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11) + (-2 + 11) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetPlayerBankCardData(playerid, pbc_pin_code),
			GetPlayerBankCardData(playerid, pbc_create_time),
			GetPlayerBankCardData(playerid, pbc_status),
			GetPlayerBankCardData(playerid, pbc_id));
		mysql_tquery(mysql, query);

		big_string[0] = EOS;
		format(big_string, sizeof(big_string),
			""cl_blue"- "cl_white"Вы успешно получили банковскую карту!\n\n\
			\\c"cl_grey"Пин-код: "cl_white"%d\n\
			\\c"cl_green"Запомните его!",
			GetPlayerBankCardData(playerid, pbc_pin_code));
		Dialog_Open(playerid, Dialog:D_NULL, DIALOG_STYLE_MSGBOX,
			"Банк", big_string,
			"Хорошо", "");
	}
	return true;
}