
DialogResponse:D_BANK_CARD_ACCEPT_CREATE(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	if(GetPlayerData(playerid, p_money) < BANK_CARD_CREATE_PRICE){
		SendErrorMessage(playerid, "У Вас недостаточно наличных денежных средств.");
		return false;
	}

	GiveMoney(playerid, -BANK_CARD_CREATE_PRICE);

	static const fmt_query[] = "INSERT INTO bank_cards (account_id, status, timer) VALUES (%d, %d, %d)";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id), BC_STATUS_WAIT, BANK_CARD_CREATE_TIME * 60);
	mysql_tquery(mysql, query, "LoadBankCardID", "i", playerid);
	return true;
}

DialogResponse:D_BANK_CARD_ENTER_PIN_CODE(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	if(strval(inputtext) != GetPlayerBankCardData(playerid, pbc_pin_code)){
		SendErrorMessage(playerid, "Вы неверно ввели пин-код!");
		Bank:ShowEnterPinCode(playerid);
		return false;
	}

	SetPlayerBankCardData(playerid, pbc_pin_code_timer, BANK_CARD_PIN_CODE_VALID_TIME);
	SendClientMessage(playerid, COLOR_GREEN, "Вы успешно ввели пин-код. Он активен в течении "#BANK_CARD_PIN_CODE_VALID_TIME" секунд.");
	return true;
}

DialogResponse:D_BANK_MENU(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	switch(listitem){
		case 0: return Bank:ShowCardInfo(playerid);
		case 1: return Bank:ShowTypeInfoList(playerid);
	}
	return true;
}

DialogResponse:D_BANK_MENU_CARD_INFO(playerid, response, listitem, inputtext[]){
	return Bank:ShowBankMenu(playerid);
}

DialogResponse:D_BANK_MENU_STATUS_LIST(playerid, response, listitem, inputtext[]){
	if(!response) return Bank:ShowBankMenu(playerid);

	switch(listitem){
		case 0: return Bank:ShowTypeInfoList(playerid);
		case 1: return Bank:ShowTypeInfo(playerid);
		case 2: return Bank:ShowChangeType(playerid);
	}
	return true;
}

DialogResponse:D_BANK_MENU_STATUS_INFO(playerid, response, listitem, inputtext[]){
	return Bank:ShowTypeInfoList(playerid);
}

DialogResponse:D_BANK_MENU_SELECT_STATUS(playerid, response, listitem, inputtext[]){
	if(!response) return Bank:ShowTypeInfoList(playerid);

	new status_index = listitem + 1;
	if(GetPlayerBankCardData(playerid, pbc_type) >= status_index){
		SendErrorMessage(playerid, "У Вас уже имеется данный статус или статус выше.");
		Bank:ShowChangeType(playerid);
		return false;
	}
	if(GetPlayerBankCardData(playerid, pbc_balance) < bank_type_prices[status_index]){
		SendErrorMessage(playerid, "У Вас недостаточно средств на счету для приобретения данного статуса.");
		Bank:ShowChangeType(playerid);
		return false;
	}

	SetPVarInt(playerid, "Bank_StatusIndex", status_index);

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Вы действительно хотите приобрести статус банковского счёта %s "cl_white"за "cl_green"%d$"cl_white"?",
		bank_card_types[status_index], bank_type_prices[status_index]);
	Dialog_Open(playerid, Dialog:D_BANK_MENU_STATUS_ACCEPT, DIALOG_STYLE_MSGBOX,
		"Банк", big_string,
		"Да", "Нет");
	big_string[0] = EOS;
	return true;
}

DialogResponse:D_BANK_MENU_STATUS_ACCEPT(playerid, response, listitem, inputtext[]){
	new status_index = GetPVarInt(playerid, "Bank_StatusIndex");
	DeletePVar(playerid, "Bank_StatusIndex");

	if(!response) return Bank:ShowChangeType(playerid);
	if(GetPlayerBankCardData(playerid, pbc_type) >= status_index){
		SendErrorMessage(playerid, "У Вас уже имеется данный статус или статус выше.");
		Bank:ShowChangeType(playerid);
		return false;
	}
	if(GetPlayerBankCardData(playerid, pbc_balance) < bank_type_prices[status_index]){
		SendErrorMessage(playerid, "У Вас недостаточно средств на счету для приобретения данного статуса.");
		Bank:ShowChangeType(playerid);
		return false;
	}

	SetPlayerBankCardData(playerid, pbc_type, status_index);

	static const fmt_query[] = "UPDATE bank_cards SET type = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerBankCardData(playerid, pbc_type), GetPlayerBankCardData(playerid, pbc_id));
	mysql_tquery(mysql, query);

	Bank:GiveMoney(playerid, -bank_type_prices[status_index]);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_green"Вы приобрели статус банковского счёта \"%s"cl_green"\" за %d$.",
		bank_card_types[status_index], bank_type_prices[status_index]);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;

	Bank:ShowMainMenu(playerid);
	return true;
}